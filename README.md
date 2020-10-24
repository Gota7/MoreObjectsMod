# More Objects Mod
Toolkit and resources for SM64DS for some custom objects.

## Custom Objects
* Galaxy Shrinking Platforms
* Silver Coins
* Rideable Yoshis
* Berries
* Invisible Walls
* 2D Level (With Camera Limiter)
* Object Lighting Modifier
* Toxic Levels
* Colored Goombas
* Noteblocks
* And More To Come!

## How It Works
MOM works from two code segments: the overlay, and the housekeeper.

The custom overlay contains all the code for custom objects in an unused section of RAM, and modifies the object and actor tables of the game to allow for custom objects.

The housekeeper hacks the original game's code to allow for loading the MOM overlay, and for using hooks and replacements to allow for cool game mechanics such as 2D levels.

## Notes
The complete guide is still a bit unorganized and uncomplete. Expect a video tutorial going through this as well in the future. Mostly the section about contributing and managing SFX needs to be done.

ALWAYS, ALWAYS, ALWAYS MAKE BACKUPS OF YOUR ROM BEFORE AND AFTER ATTEMPTING ANY INSTALL OR UPGRADE OF MOM!!!

## Installation
How to install and use MOM!

### Installing MOM Objects List (Editor)
In order for the editor to use custom objects, the editor first must have them in its listing. Copy the two files from the SM64DSe folder and replace them for your version of SM64DSe. You will now be able to place the custom objects. Please realize though that they will only work as long as MOM is inserted into the ROM.

### Installing MOM (Patch)
1. Have a CLEAN, UNOPENED EUR ROM of SM64DS.
2. Open xdeltaUI in the XDelta folder.
3. For the patch, open MOM.xdelta.
4. For the source file, select your clean ROM.
5. For the output file, name a file whatever you want with the NDS extension.
6. Click patch.
7. If it doesn't work, your ROM is either not clean, or not the EUR version.

### Installing MOM (Install Scripts)
Want to install MOM to an existing ROM without having to build it or do a lot of work yourself? Luckily, you came to the right place!

#### Tools Required:
* Latest version of SM64DSe Ultimate: https://github.com/Gota7/SM64DSe-Ultimate
* EUR version of SM64DS with NSMBe5 insertion and DLs enabled, NOT OPTIONAL!!! If using SM64DSe to enable them, please make a backup and be sure the ROM still works on your R4 afterwards!!! Patch here: http://www.mediafire.com/file/5fyes6urszhatlh/ASM+Patches.rar/file
* Nitro Studio 2: https://gota7.github.io/NitroStudio2/

#### Using Scripts
1. In SM64DSe, go to More->Import Patch.
2. Select AddOverlay.sp in the Install_Scripts folder. NEVER DO THIS MORE THAN ONCE, UNLESS IT FAILS THE FIRST TIME!
3. To be sure it worked, go to Tools->Edit Overlays, and you should now see Overlay 155. Hit "Close" and move onto Upgrading MOM.

### Installing MOM (Manually)
So you want to make changes to MOM or just install it yourself for the fun of it? Then welcome to the build process!

#### Tools Required
* Latest version of SM64DSe Ultimate: https://github.com/Gota7/SM64DSe-Ultimate
* DevKitPro with NDS Development: https://devkitpro.org/wiki/Getting_Started
* Nitro Studio 2: https://gota7.github.io/NitroStudio2/
* EUR version of SM64DS.

#### Enabling Required Patches
MOM requires insertion with NSMBe5 to be enabled and the DL patch, which can be done either in SM64DSe Ultimate (ASM Hacking->Toggle Suitability...) and (ASM Hacking->DL Patch) or a with a patch found here:
http://www.mediafire.com/file/5fyes6urszhatlh/ASM+Patches.rar/file

#### Adding The Overlay
The first step is to add a new overlay to SM64DS. This is where MOM will be stored. You only need to do this once.
1. Open your ROM in SM64DSe Ultimate, and hit Tools->Edit Overlays.
2. Right click the last overlay, and click "Add Below".
3. Note the overlay ID. Mine is 155.
4. Change the overlay RAM address to a free area big enough to house MOM (I use 0x023EC400).
5. Click "Update" then "Save Changes".

#### Compiling MOM
1. In SM64DSe, go to ASM Hacking->Compilation->Code Compiler.
2. Make sure the MOM repo is placed in a directory without any spaces.
3. Select the ASM folder in the MOM directory.
4. Change the patch settings to overlay.
5. Put in the overlay ID from earlier, and click Compile.
6. You may need to hit Clean then Compile if nonsense errors occur.
7. If no errors occur during compilation, you have now successfully installed the MOM overlay.

#### Compiling MOM House Keeper
1. Make sure the MOM repo is placed in a directory without any spaces.
2. Edit MOM.cpp in the source folder in ASM_HouseKeeper, and change 155 to your overlay ID in the call to FS_LoadOverlay.
3. Place your ROM in the ASM_HouseKeeper folder, and open NSMBe5.exe located in the 1Patcher folder.
4. DELETE THE BAK FOLDER ONLY ONCE BEFORE THE FIRST INSERTION IF YOU ARE USING YOUR OWN ROM RATHER THAN THE XDELTA ONE!!!
5. Open your ROM with NSMBe5, go to Tools/Options, then hit "Run make and insert". Hit the button below first if it produces weird errors.
6. If no errors occur during compilation, you have now successfully installed the MOM house keeper.
7. The ASM_HouseKeeper folder is your new folder for injecting ASM hacks. Any additional ARM9 code you wish to put in the game must be in here.

#### Managing Assets
The last step is to import the assets needed by the custom objects.

##### The SDAT
TODO!!! EXPLAIN SDAT MANAGEMENT!!!
So far, just replace your current SDAT with the one in the root folder of this repo. Don't worry, I'll create some clever method of importing SDAT assets later.s

##### Files
TODO!!! EXPLAIN FILE MANAGEMENT!!!

## Upgrading MOM
Of course, if you were building your mod with an original version of MOM and wish to upgrade, it would be annoying to have to start from a new XDELTA. Luckily, you can use a newly patched ROM from the XDELTA to help you upgrade.

### Upgrading SM64DSe Object DB
Follow the instructions above for installing the MOM object list.

### Using Scripts
1. In SM64DSe, go to More->Import Patch.
2. Select InstallUpgradeMOM.sp in the Install_Scripts folder.
3. Optionally, you can also install ImportTestLevels.sp for test levels as well.
4. If you do not wish to compile the house keeper yourself (although highly recommended), install ReplaceMOMHouseKeeper.sp. If you wish to inject ASM afterwards, YOU MUST USE THE ASM_HOUSEKEEPER FOLDER AS YOUR WORKING DIRECTORY WITH THE BAK FOLDER INTACT!

### Manually Upgrading
How to manually upgrade MOM without building anything.

#### Upgrading MOM Overlay
1. Open the newly patched ROM in SM64DSe.
2. In the ARM 9 Overlays tab, extract the last overlay.
3. Go to Tools->Overlay Editor, click the last overlay, and note the static init start and end table addresses.
4. Open your ROM in SM64DSe.
5. In the ARM 9 Overlays tab, replace the last overlay with the one extracted earlier.
6. Go to Tools-Overlay Editor, click the last overlay, and replace the static init start and end table addresses from the ones noted earlier.
7. Click "Update" then "Save Changes".

#### Upgrading MOM House Keeper
1. Replace the files in ASM_HouseKeeper's source files with the new source files. Mananage the code afterwards so your custom ARM9 code will work.
2. Follow the instructions above for compiling MOM's house keeper.

#### Upgrading Assets
1. Follow the instructions above for managing assets.

## Adding More
So, you want to add more content to MOM? Unfortunately, this section is TODO!!!!!!!!!!!!!!!!

## Credits
Gota7 - Mod, custom objects, sound and resource management.

Josh - Custom objects.

Starpants - Misc. ASM, General Help.

0reo - Idea.
