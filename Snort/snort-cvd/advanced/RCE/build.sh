#!/bin/bash

javac rce.java

jar cfm rce.jar MANIFEST.MF rce.class

#or run with

#java rce.java