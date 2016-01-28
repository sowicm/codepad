#!/bin/bash

file=logo.png
filename=codelite-logo

mkdir -p ../bitmaps/osx/icon.iconset
os_name=`uname -s`

#if [ "${os_name}" != "Darwin" ]; then
#    file=codelite-logo-low-res.svg
    ## 16x16 version
    #inkscape $file --without-gui --export-png=../bitmaps/16-${filename}.png --export-width=16 --export-height=16 
    #inkscape $file --without-gui --export-png=../bitmaps/16-${filename}@2x.png --export-width=32 --export-height=32 
    convert $file -resize 16x16 ../bitmaps/16-${filename}.png
    convert $file -resize 32x32 ../bitmaps/16-${filename}@2x.png
    cp ../bitmaps/16-${filename}.png ../bitmaps/osx/icon.iconset/icon_16x16.png
    cp ../bitmaps/16-${filename}@2x.png ../bitmaps/osx/icon.iconset/icon_16x16@2x.png
    
    ## 24x24 version, not needed for OSX
    #inkscape $file --without-gui --export-png=../bitmaps/24-${filename}.png --export-width=24 --export-height=24
    #inkscape $file --without-gui --export-png=../bitmaps/24-${filename}@2x.png --export-width=48 --export-height=48 
    convert $file -resize 24x24 ../bitmaps/24-${filename}.png
    convert $file -resize 48x48 ../bitmaps/24-${filename}@2x.png

    cp ../bitmaps/24-${filename}.png ../bitmaps/24-${filename}-no-shadow.png
    cp ../bitmaps/24-${filename}@2x.png ../bitmaps/24-${filename}-no-shadow@2x.png

    ## 32x32 version
    #inkscape $file --without-gui --export-png=../bitmaps/32-${filename}.png --export-width=32 --export-height=32 
    #inkscape $file --without-gui --export-png=../bitmaps/32-${filename}@2x.png --export-width=64 --export-height=64 
    convert $file -resize 32x32 ../bitmaps/32-${filename}.png
    convert $file -resize 64x64 ../bitmaps/32-${filename}@2x.png
    cp ../bitmaps/32-${filename}.png ../bitmaps/osx/icon.iconset/icon_32x32.png
    cp ../bitmaps/32-${filename}@2x.png ../bitmaps/osx/icon.iconset/icon_32x32@2x.png

    ## 64x64 version
    #inkscape $file --without-gui --export-png=../bitmaps/64-${filename}.png --export-width=64 --export-height=64 
    #inkscape $file --without-gui --export-png=../bitmaps/64-${filename}@2x.png --export-width=128 --export-height=128 
    convert $file -resize 64x64 ../bitmaps/64-${filename}.png
    convert $file -resize 128x128 ../bitmaps/64-${filename}@2x.png


    ## Use the hi-res image from hereon
    ## 128x128 version
#    file=codelite-logo.svg
    #inkscape $file --without-gui --export-png=../bitmaps/128-${filename}.png --export-width=128 --export-height=128 
    #inkscape $file --without-gui --export-png=../bitmaps/128-${filename}@2x.png --export-width=256 --export-height=256 
    convert $file -resize 128x128 ../bitmaps/128-${filename}.png
    convert $file -resize 256x256 ../bitmaps/128-${filename}@2x.png
    cp ../bitmaps/128-${filename}.png ../bitmaps/osx/icon.iconset/icon_128x128.png
    cp ../bitmaps/128-${filename}@2x.png ../bitmaps/osx/icon.iconset/icon_128x128@2x.png

    ## 256x256 version
    #inkscape $file --without-gui --export-png=../bitmaps/256-${filename}.png --export-width=256 --export-height=256 
    #inkscape $file --without-gui --export-png=../bitmaps/256-${filename}@2x.png --export-width=512 --export-height=512 
    convert $file -resize 256x256 ../bitmaps/256-${filename}.png
    convert $file -resize 512x512 ../bitmaps/256-${filename}@2x.png
    cp ../bitmaps/256-${filename}.png ../bitmaps/osx/icon.iconset/icon_256x256.png
    cp ../bitmaps/256-${filename}@2x.png ../bitmaps/osx/icon.iconset/icon_256x256@2x.png

    ## 512x512 version
    #inkscape $file --without-gui --export-png=../bitmaps/512-${filename}.png --export-width=512 --export-height=512 
    #inkscape $file --without-gui --export-png=../bitmaps/512-${filename}@2x.png --export-width=1024 --export-height=1024 
    convert $file -resize 512x512 ../bitmaps/512-${filename}.png
    convert $file -resize 1024x1024 ../bitmaps/512-${filename}@2x.png
    cp ../bitmaps/512-${filename}.png ../bitmaps/osx/icon.iconset/icon_512x512.png
    cp ../bitmaps/512-${filename}@2x.png ../bitmaps/osx/icon.iconset/icon_512x512@2x.png
#fi

png2ico ../LiteEditor/res/codepad-logo.ico ../bitmaps/16-${filename}.png ../bitmaps/24-${filename}.png ../bitmaps/32-${filename}.png ../bitmaps/64-${filename}.png ../bitmaps/128-${filename}.png #../bitmaps/256-${filename}.png

cd ../bitmaps

##
## Create a zip file named codelite-bitmaps.zip
##
rm -f ../Runtime/codelite-bitmaps.zip
zip ../Runtime/codelite-bitmaps.zip *.png

#if [ "${os_name}" == "Darwin" ]; then
    cd osx
    echo iconutil -c icns icon.iconset/
    iconutil -c icns icon.iconset/
#fi
