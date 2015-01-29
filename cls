rm -rf autom4te.cache
rm -rf autoscan-2.69.log
rm -rf aclocal.m4
rm -rf configure.scan
rm -rf m4
rm -rf config.*
rm -rf configure
rm -rf ltmain.sh
rm -rf COPYING
rm -rf depcomp
rm -rf INSTALL
rm -rf libtool
rm -rf missing
rm -rf compile
rm -rf Makefile.in
rm -rf Makefile
rm -rf install-sh
rm -rf stamp-h1
rm -rf *.log *.cei

for dir in */
do
	
	if [[ $dir != ".git" ]];then

		IFS="/"
		declare -a Array=($dir)
	
		cd $(echo "$Array")
		rm -f Makefile.in
		rm -f  Makefile
		rm -f  *.exe
		rm -f  *.o
		rm -f *.ini
		rm -f *.lo
		rm -f *.s
		rm -f *.i
		rm -f *.la
		rm -f *.log *.cei
		rm -rf .deps
		rm -rf .libs
		rm -f *.log
		
		for sdir in */
		
		do
			if [[ "$sdir" ]]; then
				if [[ $sdir != "*/" ]];then
					cd "$sdir"
					rm -f Makefile.in
					rm -f Makefile
					rm -f *.exe
					rm -f *.o
					rm -f *.s
					rm -f *.i
					rm -f *.ini
					rm -f *.lo
					rm -f *.la
					rm -f *.log *.cei
					rm -rf .deps
					rm -rf .libs
					rm -f *.log
					cd ..
				fi
			fi
		done
		
	cd ..
	
	fi
done
