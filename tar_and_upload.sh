#!/bin/bash
LOGIN=your_username
FILES=main.c

if test -z "$1"
then
    echo STAGE argument not given
else 
	echo MAKING STAGE${1} TARBALL
	tar -cjf ${LOGIN}.etap${1}.tar.bz2 ${FILES}
	echo CONTENT OF ${LOGIN}.etap${1}.tar.bz2 :
	tar -tjf ${LOGIN}.etap${1}.tar.bz2 ${FILES}
	scp ${LOGIN}.etap${1}.tar.bz2 ${LOGIN}@ssh.mini.pw.edu.pl:/home2/samba/borkowskim/unix/
fi