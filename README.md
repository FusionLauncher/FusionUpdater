# FusionClientUpdater
Build instructions (Linux):

Requires: build-essential, qt4-default, git

'''
git clone https://github.com/lolsatbirds/LibFusion
cd LibFusion
qmake && make
cd ..
git clone https://github.com/lolsatbirds/FusionClientUpdater
cd FusionClientUpdater
qmake && make
LD_LIBRARY_PATH=../LibFusion ./FusionClientUpdater
'''
