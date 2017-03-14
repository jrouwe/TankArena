# Make file for TANK ARENA

# Do yourself a favor:
#
# * Install Tank Arena in \tank.
# * Look in the file MAKEDIR.

ZIP = zip -kr9
PKZIP = pkzip -exrP
UNZIPSFX = \utils\unzipsfx.exe

# No need to change anything after this

LIBS = game setup edit world tanks objects data check graph hardware

tank: $(LIBS:%=%.a)
	gcc -g -o ../tank.exe $(LIBS:%=%/libout.a) -lalleg -lemu

clean: $(LIBS:%=%.clean)

%.a:
	@cd $(@:%.a=%)
	@make -f ../makedir libout.a
	@cd ..

%.clean:
	@cd $(@:%.clean=%)
	@make -f ../makedir clean
	@cd ..

strip:
	exe2coff ../tank.exe
	strip ../tank
	coff2exe ../tank
	del ..\tank

# Now some functions to create the zip files.

tanku:
	@cd ..
	$(ZIP) zip/tanku *.exe *.bat *.doc *.hlp data/*.* maps/*.map
	copy /b $(UNZIPSFX)+zip\tanku.zip zip\tanku.exe
	zip -A zip/tanku.exe
	del zip\tanku.zip
	@cd src

title:
	@cd ..
	$(ZIP) zip/title fli/logo.fli fli/title.fli
	copy /b $(UNZIPSFX)+zip\title.zip zip\title.exe
	zip -A zip/title.exe
	del zip\title.zip
	@cd src

extra:
	@cd ..
	$(ZIP) zip/extra fli/thunder.fli fli/victory.fli
	copy /b $(UNZIPSFX)+zip\extra.zip zip\extra.exe
	zip -A zip/extra.exe
	del zip\extra.zip
	@cd src

src:
	@cd ..
	del zip\src.zip
	$(ZIP) zip/src src -x *.o *.a
	@cd src

pics:
	@cd ..
	del zip\pics.zip
	$(PKZIP) zip/pics bat/*.* 23pics/*.* 2hpics/*.* 2vpics/*.* 32pics/*.*
	$(PKZIP) zip/pics 3pics/*.* 4hpics/*.* 4pics/*.* pics/*.* maps/*.?
	@cd src


