#!/bin/bash
hadoop fs -rm -r /simple.output
hadoop com.sun.tools.javac.Main FoF.java
jar cf FoF.jar FoF*.class
hadoop jar ./FoF.jar FoF /simple.input /simple.output
hadoop fs -cat /simple.output/part*