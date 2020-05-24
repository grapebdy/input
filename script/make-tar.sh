#!/bin/sh

PROJ_OUT=$1
RELNAME=$2
cd $PROJ_OUT
tar czmPf $RELNAME-bin.tar.gz $RELNAME
