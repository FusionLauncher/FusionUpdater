# FusionClientUpdater
Build instructions (**Linux**):

**Requires**: **build-essential**, **qt4-default**, **git**

```
git clone https://github.com/FusionLauncher/LibFusion
cd LibFusion
qmake && make
cd ..
git clone https://github.com/FusionLauncher/FusionUpdater
cd FusionUpdater
qmake && make
LD_LIBRARY_PATH=../LibFusion ./FusionClientUpdater
```
