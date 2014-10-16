struct uri {
	char* prefix; // http, ftp, etc.
	char* domain;
	char* path; // Everything following the domain
};

struct uri* getURI(char* input); 
