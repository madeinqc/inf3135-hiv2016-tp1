tp1: tp1.o
	gcc -o tp1 tp1.o

tp1.o: tp1.c
	gcc -c tp1.c

clean:
	rm tp1.o
	rm tp1

database:
	if [ -d data ];\
	then\
		rm -r data;\
	fi;
	mkdir data
	curl http://download.geonames.org/export/dump/cities15000.zip -o data/cities15000.zip
	curl http://download.geonames.org/export/dump/countryInfo.txt -o data/countryInfo.txt
	unzip data/cities15000.zip -d data
	rm data/cities15000.zip
