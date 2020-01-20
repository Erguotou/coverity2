all:
	make -C Debug clean
	cov-build --dir cov-int make -C Debug
	tar czvf forcov.tgz cov-int