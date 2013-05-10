#include <stdio.h>

#include <curl/curl.h>

int main(int argc, char *argv[])
{
	char buf[128];
	CURLcode res;
	CURL *curl;
	int use_pop = 1;

	if (argc < 4) {
		fprintf(stderr, "syntax %s <pop> <user> <pwd> [imap]\n",argv[0]);
		return -1;
	}

	use_pop = (argc == 4);
	curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "can't init curl\n");
		return -1;
	}

	/* Set username and password */
	snprintf(buf, 128, "%s:%s", argv[2], argv[3]);
	curl_easy_setopt(curl, CURLOPT_USERPWD, buf);

	/* This will list every message of the given mailbox */
	snprintf(buf, 128, "%ss://%s@%s/%s", use_pop? "pop3":"imap", argv[2], argv[1], use_pop?"":"INBOX");
	printf("XXXX %s\n", buf);
	curl_easy_setopt(curl, CURLOPT_URL, buf);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

	/* Perform the request, res will get the return code */ 
	res = curl_easy_perform(curl);
	/* Check for errors */ 
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			  curl_easy_strerror(res));

	snprintf(buf, 128, "%ss://%s@%s/%s1", use_pop? "pop3":"imap", argv[2], argv[1], use_pop?"":"INBOX;UID=");
	printf("XXXX %s\n", buf);
	curl_easy_setopt(curl, CURLOPT_URL, buf);
	res = curl_easy_perform(curl);
	/* Check for errors */ 
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			  curl_easy_strerror(res));

	/* always cleanup */ 
	curl_easy_cleanup(curl);
	return 0;
}
