// zip utilities --------------------------------------------------------------
// - rlyeh, public domain
//
// paths are given in `zipfilepath` syntax: "c:/prj/my.zip/mydir/myfile"

API bool  zip(const char *fullpath, const char *bin, int len);
API char *unzip(const char *fullpath, int *len); // must free() after use

#if CODE

bool zip(const char *fullpath, const char *bin, int len) {
	const char *filename = strstr(fullpath, ".zip/"); filename += !!filename * 5;
	if( !filename ) return false;
	const char *zipfile = va("%.*s", (int)(filename - 1 - fullpath), fullpath);

	struct zip_t *z = zip_open(zipfile, "a+b");
	if( z ) {
		unsigned compress_level = 9;
		const char *entryname = filename;
		const char *comment = "";
	    bool ok = zip_append_mem(z, entryname, comment, bin, len, compress_level);
		zip_close(z);
		return ok;
	}

	return false;
}

char *unzip(const char *fullpath, int *len) { // must free() after use
	const char *filename = strstr(fullpath, ".zip/"); filename += !!filename * 5;
	if( !filename ) return NULL;
	const char *zipfile = va("%.*s", (int)(filename - 1 - fullpath), fullpath);

	char *bin = 0;
	struct zip_t *z = zip_open(zipfile, "rb");
	if( z ) {
	    int index = zip_find(z, filename);
	    if( index >= 0 ) {
	        bin = zip_extract(z, index);
	        if(len) *len = zip_size(z, index);
	    }
	    zip_close(z);
	}

	return bin;
}

#endif
