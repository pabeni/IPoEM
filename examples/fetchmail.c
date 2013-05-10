/*
This file is part of IPoEM.

IPoEM is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

IPoEM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with IPoEM. If not, see <http://www.gnu.org/licenses/>.
*/

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
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

	/* Get message list*/ 
	snprintf(buf, 128, "%ss://%s@%s/%s", use_pop? "pop3":"imap", argv[2], argv[1], use_pop?"":"INBOX");
	printf("XXXX %s\n", buf);
	curl_easy_setopt(curl, CURLOPT_URL, buf);
	res = curl_easy_perform(curl);
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

	/* Get first message content */
	snprintf(buf, 128, "%ss://%s@%s/%s1", use_pop? "pop3":"imap", argv[2], argv[1], use_pop?"":"INBOX;UID=");
	printf("XXXX %s\n", buf);
	curl_easy_setopt(curl, CURLOPT_URL, buf);
	res = curl_easy_perform(curl);
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

	curl_easy_cleanup(curl);
	return 0;
}
