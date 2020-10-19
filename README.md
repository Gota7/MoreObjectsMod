# More Objects Mod
Toolkit and resources for SM64DS for some custom objects.

## Custom Objects
* Galaxy Shrinking Platforms
* Silver Coins
* Rideable Yoshis
* Berries
* Colored Goombas
* Shy Guys
* 2D Level (With Camera Limiter)
* Object Lighting Modifier
* Conditional Cutscene Loader
* And More To Come!

## How It Works
MOM works from two code segments: the overlay, and the housekeeper.

The custom overlay contains all the code for custom objects in an unused section of RAM, and modifies the object and actor tables of the game to allow for custom objects.

The housekeeper hacks the original game's code to allow for loading the MOM overlay, and for using hooks and replacements to allow for cool game mechanics such as 2D levels.

## Installation
How to install and use MOM!

### Installing MOM Objects List (Editor)
In order for the editor to use custom objects, the editor first must have them in its listing. Copy the two files from the SM64DSe folder and replace them for your version of SM64DSe. You will now be able to place the custom objects. Please realize though that they will only work as long as MOM is inserted into the ROM.

### Installing MOM (Patch)
TODO: XDELTA PATCH INSTRUCTIONS!!!

### Installing MOM (Manually)
So you want to make changes to MOM or just install it yourself for the fun of it? Then welcome to the build process!

#### Tools Required
* Latest version of SM64DSe Ultimate: 
* DevKitPro with NDS Development: 
* Nitro Studio 2: 
* EUR version of SM64DS.

#### Enabling Required Patches
MOM requires insertion with NSMBe5 to be enabled, which can be done either in SM64DSe Ultimate (ASM Hacking->Toggle Suitability...) or a with a patch found here: 

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

##### Files
TODO!!! EXPLAIN FILE MANAGEMENT!!!

## Upgrading MOM
Of course, if you were building your mod with an original version of MOM and wish to upgrade, it would be annoying to have to start from a new XDELTA. Luckily, you can use a newly patched ROM from the XDELTA to help you upgrade.

### Upgrading MOM Overlay
1. Open the newly patched ROM in SM64DSe.
2. In the ARM 9 Overlays tab, extract the last overlay.
3. Go to Tools->Overlay Editor, click the last overlay, and note the static init start and end table addresses.
4. Open your ROM in SM64DSe.
5. In the ARM 9 Overlays tab, replace the last overlay with the one extracted earlier.
6. Go to Tools-Overlay Editor, click the last overlay, and replace the static init start and end table addresses from the ones noted earlier.
7. Click "Update" then "Save Changes".

### Upgrading MOM House Keeper
1. Replace the files in ASM_HouseKeeper's source files with the new source files. Mananage the code afterwards so your custom ARM9 code will work.
2. Follow the instructions above for compiling MOM's house keeper.

### Upgrading Assets
1. Follow the instructions above for managing assets.

## Adding More
So, you want to add more content to MOM? Unfortunately, this section is TODO!!!!!!!!!!!!!!!!

## Credits
Gota7 - Mod, custom objects, sound and resource management.

Josh - Custom objects.

Starpants - Misc. ASM, General Help.

0reo - Idea.
