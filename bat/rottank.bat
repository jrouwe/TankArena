@echo off

cd \tank\pics

copy tan%1-1 tan%1-5
copy tan%1-2 tan%1-6
copy tan%1-3 tan%1-7
copy tan%1-2 tan%1-8
copy tan%1-1 tan%1-9
copy tan%1-4 tan%1-10
copy tan%1-3 tan%1-11
copy tan%1-2 tan%1-12
copy tan%1-1 tan%1-13
copy tan%1-4 tan%1-14
copy tan%1-3 tan%1-15
copy tan%1-4 tan%1-16

..\utils\bdraw tan%1-5 33 33 9y
..\utils\bdraw tan%1-6 33 33 9y
..\utils\bdraw tan%1-7 33 33 x
..\utils\bdraw tan%1-8 33 33 x
..\utils\bdraw tan%1-9 33 33 x
..\utils\bdraw tan%1-10 33 33 9
..\utils\bdraw tan%1-11 33 33 9
..\utils\bdraw tan%1-12 33 33 9
..\utils\bdraw tan%1-13 33 33 9
..\utils\bdraw tan%1-14 33 33 y
..\utils\bdraw tan%1-15 33 33 y9
..\utils\bdraw tan%1-16 33 33 9x

copy tan%1-17 tan%1-21
copy tan%1-18 tan%1-22
copy tan%1-19 tan%1-23
copy tan%1-18 tan%1-24
copy tan%1-17 tan%1-25
copy tan%1-20 tan%1-26
copy tan%1-19 tan%1-27
copy tan%1-18 tan%1-28
copy tan%1-17 tan%1-29
copy tan%1-20 tan%1-30
copy tan%1-19 tan%1-31
copy tan%1-20 tan%1-32

..\utils\bdraw tan%1-21 33 33 9y
..\utils\bdraw tan%1-22 33 33 9y
..\utils\bdraw tan%1-23 33 33 x
..\utils\bdraw tan%1-24 33 33 x
..\utils\bdraw tan%1-25 33 33 x
..\utils\bdraw tan%1-26 33 33 9
..\utils\bdraw tan%1-27 33 33 9
..\utils\bdraw tan%1-28 33 33 9
..\utils\bdraw tan%1-29 33 33 9
..\utils\bdraw tan%1-30 33 33 y
..\utils\bdraw tan%1-31 33 33 y9
..\utils\bdraw tan%1-32 33 33 9x

copy tan%1-33 tan%1-37
copy tan%1-34 tan%1-38
copy tan%1-35 tan%1-39
copy tan%1-34 tan%1-40
copy tan%1-33 tan%1-41
copy tan%1-36 tan%1-42
copy tan%1-35 tan%1-43
copy tan%1-34 tan%1-44
copy tan%1-33 tan%1-45
copy tan%1-36 tan%1-46
copy tan%1-35 tan%1-47
copy tan%1-36 tan%1-48

..\utils\bdraw tan%1-37 33 33 9y
..\utils\bdraw tan%1-38 33 33 9y
..\utils\bdraw tan%1-39 33 33 x
..\utils\bdraw tan%1-40 33 33 x
..\utils\bdraw tan%1-41 33 33 x
..\utils\bdraw tan%1-42 33 33 9
..\utils\bdraw tan%1-43 33 33 9
..\utils\bdraw tan%1-44 33 33 9
..\utils\bdraw tan%1-45 33 33 9
..\utils\bdraw tan%1-46 33 33 y
..\utils\bdraw tan%1-47 33 33 y9
..\utils\bdraw tan%1-48 33 33 9x

cd ..

utils\dat
