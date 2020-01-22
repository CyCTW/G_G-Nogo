#!/bin/bash
gogui-twogtp -white "/tcg/files/pj-4-test" -black "nogo" -size 9 -auto -games 100 -sgffile test1 -verbose
gogui-twogtp -black "/tcg/files/pj-4-test" -white "nogo" -size 9 -auto -games 100 -sgffile test2 -verbose

