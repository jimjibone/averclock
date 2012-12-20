(
	echo "// This allow the clock to the set to the current time automatically"
	echo "#define MINUTES  $(date +%M)"
	echo "#define HOURS    $(date +%k)"
	echo "#define SECONDS  $(date +%S)"
) > src/current_time.h

ino build && ino upload

# better reset to zero
git checkout src/current_time.h
