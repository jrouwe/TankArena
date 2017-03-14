@echo off

cd pics

copy tan%1-3 tan%1-50
copy tan%1-3 tan%1-51
copy tan%1-3 tan%1-52
copy tan%1-3 tan%1-53
copy tan%1-3 tan%1-54

cd..

utils\bdraw pics\tan%1-50 33 33 z
utils\bdraw pics\tan%1-51 33 33 zz
utils\bdraw pics\tan%1-52 33 33 zzz
utils\bdraw pics\tan%1-53 33 33 zzzz
utils\bdraw pics\tan%1-54 33 33 zzzzz

utils\dat
