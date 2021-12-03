// Stream-based output utilities by pants64

#ifndef O_STREAM_INCLUDED
#define O_STREAM_INCLUDED

#include <cstdint>

class ostream
{
	static constexpr std::size_t buffer_size = 120;
	static char buffer[buffer_size];

public:

	[[gnu::naked, gnu::target("thumb")]]
	static void flush(...)
	{
		asm volatile (R"(
			mov   r12,r12
			b     end
			.hword 0x6464
			.hword 0x0000
		.weak _ZN7ostream6bufferE
		_ZN7ostream6bufferE:
			.fill 120
			.hword 0
		end:
			bx lr
		)");
	}

	static void set_buffer(const char* str)
	{
		for (std::size_t i = 0; i < buffer_size; i++)
		{
			if ((buffer[i] = str[i]) == '\0')
				break;
		}
	}
	
	const ostream& operator<<(const char* str) const
	{
		set_buffer(str);
		flush();
		return *this;
	}

	const ostream& operator<<(char character) const
	{
		buffer[0] = character;
		buffer[1] = '\0';

		flush();

		return *this;
	}

	const ostream& operator<<(uint32_t val) const
	{
		set_buffer("0x%r0%");
		flush(val);

		return *this;
	}

	const ostream& operator<<(uint64_t val) const
	{
		set_buffer("0x%r1%%r0%");
		flush(val);

		return *this;
	}

	template<typename T>
	const ostream& operator<<(T* ptr)       const { return *this << reinterpret_cast<uint32_t>(ptr); }
	const ostream& operator<<(char* ptr)    const { return *this << static_cast<const char*>(ptr); }
	const ostream& operator<<(unsigned val) const { return *this << static_cast<uint32_t>(val); }
	const ostream& operator<<(bool b)       const { return *this << (b ? "true" : "false"); }

	const ostream& operator<<(int val) const
	{
		if (val < 0)
		{
			set_buffer("-0x%r0%");
			flush(-val);
		}
		else
		{
			set_buffer("0x%r0%");
			flush(val);	
		}
		return *this;
	}
	
	const ostream& operator<<(int64_t val) const
	{
		if (val < 0)
		{
			set_buffer("-0x%r1%%r0%");
			flush(-val);
		}
		else
		{
			set_buffer("0x%r1%%r0%");
			flush(val);	
		}
		return *this;
	}
}
constexpr cout;

#endif