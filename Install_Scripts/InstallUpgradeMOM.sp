# Modify the MOM overlay.
edit_overlay 155 23EC400 23F7D74 23F7D78 0 0
replace_overlay 155 ../ASM/Overlay_155.bin

# Edit filenames.
rename 0 000_galaxyShrinkPlatformFrame.bmd
rename 1 001_galaxyShrinkPlatform.bmd
rename 2 002_galaxyShrinkPlatform.kcl
rename 3 003_silverCoin.bmd
rename 4 004_invisibleWall.bmd
rename 5 005_invisibleWall.kcl
rename 6 006_berry.bmd
rename 7 007_berryStem.bmd
rename 8 008_riding.bca
rename 9 009_coloredGoomba.bmd
rename A 010_coloredGoomba.btp
rename B 011_noteblock.bmd
rename C 012_noteblock.kcl
rename D 013_shyGuy.bmd
rename E 014_shyGuyWait.bca
rename F 015_shyGuyWalk.bca
rename 10 016_shyGuyRun.bca
rename 11 017_shyGuyFreeze.bca
rename 12 018_lstar.bmd
rename 13 019_lsWait.bca
rename 14 020_lsLaunch.bca
rename 15 021_magikoopa.bmd
rename 16 022_kamella.bmd
rename 17 023_magikoopaAppear.bca
rename 18 024_magikoopaWave.bca
rename 19 025_magikoopaShoot.bca
rename 1A 026_magikoopaPoof.bca
rename 1B 027_magikoopaWait.bca
rename 1C 028_magikoopaHurt.bca
rename 1D 029_magikoopaDefeat.bca
rename 1E 030_magikoopaMagic.tps
rename 1F 031_icicle.bmd
rename 20 032_coloredGoomba2.bmd
rename 21 033_coloredGoomba2.btp
rename 22 034_yoshiNPC.bmd
rename 23 035_yoshiNPCWait.bca
rename 24 036_coloredPipe.bmd
rename 25 037_coloredPipe.kcl
rename 26 038_characterBlockMario.bmd
rename 27 039_characterBlockLuigi.bmd
rename 28 040_characterBlockWario.bmd
rename 29 041_characterBlockYoshi.bmd
rename 2A 042_characterBlockAll.kcl
rename 2B 043_characterBlockAll.bca
rename 2C 044_characterBlockTransMario.bmd
rename 2D 045_characterBlockTransLuigi.bmd
rename 2E 046_characterBlockTransWario.bmd
rename 2F 047_characterBlockTransYoshi.bmd
rename 30 048_characterBlockTransAll.bca
rename 31 049_saveBlock.bmd
rename 32 050_saveBlock.btp
rename 33 051_saveBlock.kcl
rename 34 052_starChip.bmd
rename 35 053_doorBlocker.bmd
rename 36 054_doorBlocker.kcl
rename 37 055_coloredCoin.bmd

# Replace assets.
replace 0 ../Objects/GalaxyShrinkPlatform/ModelCollision/galaxyShrinkPlatformFrame.bmd
replace 1 ../Objects/GalaxyShrinkPlatform/ModelCollision/galaxyShrinkPlatform.bmd
replace 2 ../Objects/GalaxyShrinkPlatform/ModelCollision/galaxyShrinkPlatform.kcl
replace 3 ../Objects/SilverCoin/Model/silverCoin.bmd
replace 4 ../Objects/InvisibleWall/ModelCollision/invisibleWall.bmd
replace 5 ../Objects/InvisibleWall/ModelCollision/invisibleWall.kcl
replace 6 ../Objects/Berry/Model/berry.bmd
replace 7 ../Objects/Berry/Model/berryStem.bmd
replace 8 ../Objects/YoshiRide/Anim/riding.bca
replace 9 ../Objects/ColoredGoombas/Model/kuribo_model.bmd
replace A ../Objects/ColoredGoombas/TexAnim/kuribo_colorsw.btp
replace B ../Objects/Noteblock/ModelCollision/noteblock.bmd
replace C ../Objects/Noteblock/ModelCollision/noteblock.kcl
replace D ../Objects/ShyGuy/Model/shyGuy.bmd
replace E ../Objects/ShyGuy/Anim/shyGuyWait.bca
replace F ../Objects/ShyGuy/Anim/shyGuyWalk.bca
replace 10 ../Objects/ShyGuy/Anim/shyGuyRun.bca
replace 11 ../Objects/ShyGuy/Anim/shyGuyFreeze.bca
replace 12 ../Objects/LaunchStar/Model/lstar.bmd
replace 13 ../Objects/LaunchStar/Anim/lsWait.bca
replace 14 ../Objects/LaunchStar/Anim/lsLaunch.bca
replace 15 ../Objects/Magikoopa/Model/magikoopa.bmd
replace 16 ../Objects/Magikoopa/Model/kamella.bmd
replace 17 ../Objects/Magikoopa/Anim/magikoopaAppear.bca
replace 18 ../Objects/Magikoopa/Anim/magikoopaWave.bca
replace 19 ../Objects/Magikoopa/Anim/magikoopaShoot.bca
replace 1A ../Objects/Magikoopa/Anim/magikoopaPoof.bca
replace 1B ../Objects/Magikoopa/Anim/magikoopaWait.bca
replace 1C ../Objects/Magikoopa/Anim/magikoopaHurt.bca
replace 1D ../Objects/Magikoopa/Anim/magikoopaDefeat.bca
replace 1E ../Objects/Magikoopa/Particle/magikoopaMagic.tps
replace 1F ../Objects/FallingIcicle/Model/icicle.bmd
replace 20 ../Objects/ColoredGoombas2/Model/kuribo_model.bmd
replace 21 ../Objects/ColoredGoombas2/TexAnim/kuribo_colorsw.btp
replace 22 ../Objects/YoshiNPC/Model/yoshi_model.bmd
replace 23 ../Objects/YoshiNPC/Anim/Y_wait.bca
replace 24 ../Objects/ColoredPipe/ModelCollision/obj_dokan.bmd
replace 25 ../Objects/ColoredPipe/ModelCollision/obj_dokan.kcl
replace 26 ../Objects/CharacterBlock/ModelCollision/obj_cap_box_m.bmd
replace 27 ../Objects/CharacterBlock/ModelCollision/obj_cap_box_l.bmd
replace 28 ../Objects/CharacterBlock/ModelCollision/obj_cap_box_w.bmd
replace 29 ../Objects/CharacterBlock/ModelCollision/obj_cap_box_y.bmd
replace 2A ../Objects/CharacterBlock/ModelCollision/hatena_box.kcl
replace 2B ../Objects/CharacterBlock/Anim/hatena_karabox_empty.bca
replace 2C ../Objects/CharacterBlock/ModelCollision/hatena_karabox_m.bmd
replace 2D ../Objects/CharacterBlock/ModelCollision/hatena_karabox_l.bmd
replace 2E ../Objects/CharacterBlock/ModelCollision/hatena_karabox_w.bmd
replace 2F ../Objects/CharacterBlock/ModelCollision/hatena_karabox_y.bmd
replace 30 ../Objects/CharacterBlock/Anim/hatena_karabox.bca
replace 31 ../Objects/SaveBlock/ModelCollision/saveBlock.bmd
replace 32 ../Objects/SaveBlock/TexAnim/saveBlock.btp
replace 33 ../Objects/SaveBlock/ModelCollision/saveBlock.kcl
replace 34 ../Objects/StarChip/Model/starChip.bmd
replace 35 ../Objects/DoorBlocker/ModelCollision/door_blocker.bmd
replace 36 ../Objects/DoorBlocker/ModelCollision/door_blocker.kcl
replace 37 ../Objects/ColoredCoin/Model/coloredCoin.bmd