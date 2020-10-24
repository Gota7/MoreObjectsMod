# Modify the MOM overlay.
edit_overlay 155 23EC400 23F0F94 23F0F98 0 0
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