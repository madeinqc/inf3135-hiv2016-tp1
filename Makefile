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
		rm data;\
	fi;
	mkdir data
	cd data
	curl http://download.geonames.org/export/dump/cities15000.zip -o cities15000.zip
	curl http://download.geonames.org/export/dump/countryInfo.txt -o countryInfo.txt
	unzip cities15000.zip
	rm cities15000.zip
	cd ..
	
