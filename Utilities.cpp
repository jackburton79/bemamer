#include <Entry.h>
#include <List.h>
#include <String.h>

#include <cstdlib>

char**
strsplit(const char *string, const char *delimiter, int max_tokens)
{
	BString longString(string);
	BList list;
	int n, max = 0;
	
	while (max <= max_tokens && (n = longString.FindFirst(delimiter)) != B_ERROR)	{
		BString tmp;
		longString.MoveInto(tmp, 0, n + 1);
		tmp.RemoveLast(delimiter);	
		list.AddItem(strdup(tmp.String()));
		max++;
	}
	int count = list.CountItems();
	char **ret = new char* [count + 1];

	for (int x = 0; x < count; x++)
		ret[x] = reinterpret_cast<char*>(list.ItemAt(x));
	
	ret[count] = NULL;
	
	return ret;
}


void
strfreev(char **str_array)
{
	if (str_array) {
		int i = 0;
		
		while (str_array[i] != NULL) {
			free(str_array[i]);
			str_array[i] = NULL;
			i++;
		}
		delete[] str_array;
		str_array = NULL;
	}
}
