#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/cryptoclear.ico

convert ../../src/qt/res/icons/cryptoclear-16.png ../../src/qt/res/icons/cryptoclear-32.png ../../src/qt/res/icons/cryptoclear-48.png ${ICON_DST}
