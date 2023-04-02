#include <iostream>
#include "TextFilesManager.h"



int main()
{
	TextFilesManager filesManager(8);

	filesManager.AddFile("pipiPupu.txt", 20, 10, 40, 01, 04, 2023, DEFAULT_RIGHTS);
	filesManager.EditFile("pipiPupu.txt","At least I Tried. Please Have mercy on me!", 20, 10, 40, 01, 04, 2023, User::Owner);
	filesManager.AppendToFile("pipiPupu.txt", "May I get more than troika, please! I ve been coiding for 4 days non-stop ", 23, 59, 40, 01, 04, 2023, User::Owner);
	filesManager.DeleteFile("pipiPupu.txt");
}