#!/usr/bin/env python
import os
import zipfile

def zipdir(path, zip):
	ext = ('.cpp','.h','.hpp')

	for file in os.listdir(path):
		if file.endswith(ext):
			zipf.write(path + '/' + file)

if __name__ == '__main__':
	zipf = zipfile.ZipFile('Game.zip', 'w', zipfile.ZIP_DEFLATED)
	zipdir('.', zipf)
	zipdir('./tools/', zip);
	zipf.close()