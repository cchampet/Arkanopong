#include "bloc.h"



void drawingBloc() {
	glBegin(GL_QUADS);
	glTexCoord2f(1., 0.);
	glVertex2f(1., 0.);
	glTexCoord2f(1., 1.);
	glVertex2f(1., -1.);
	glTexCoord2f(0., 1.);
	glVertex2f(0., -1.);
	glTexCoord2f(0., 0.);
	glVertex2f(0., 0.);
	glEnd();
}



Bloc * AllocBloc(Position p, typeBloc type, float size, const char * nameImage) {
	Bloc * bloc = (Bloc*)malloc(sizeof(Bloc));
	bloc->position = p;
	bloc->type = type;
	bloc->size = size;
	bloc->texture = TextureCreation(&bloc->texture, nameImage);

	return bloc;
}



NodeBloc * OneBlocInTheList(Bloc * bloc) {
	if (bloc == NULL) {
		fprintf(stderr, "Pointer NULL for bloc in function OneBlocInTheList \n");
		exit(EXIT_FAILURE);
	}
	NodeBloc * node = (NodeBloc*)malloc(sizeof(NodeBloc));
	node->data = bloc;
	node->previous = NULL;
	node->next = NULL;
	return node;
}



void AddBlocToList(Bloc * bloc, ListBlocs * list) {
	if (bloc == NULL) {
		fprintf(stderr, "Pointer NULL for bloc in function AddBlocToList \n");
		exit(EXIT_FAILURE);
	}
	if (list == NULL) {
		fprintf(stderr, "Pointer NULL for list in function AddBlocToList \n");
		exit(EXIT_FAILURE);
	}
	/* if the list is empty */
	if ((*list)->data == NULL) {
		(*list) = OneBlocInTheList(bloc);
	}
	else {
		ListBlocs listTemp = *list;
		while (listTemp->next != NULL) {
			listTemp = listTemp->next;
		}
		listTemp->next = OneBlocInTheList(bloc);
		(listTemp->next)->previous = listTemp;
	}
}



void ListBlocsCreation(char * filepath, ListBlocs * list, int theme) {
	if (list == NULL) {
		fprintf(stderr, "Pointer NULL for list in function ListBlocsCreation \n");
		exit(EXIT_FAILURE);
	}

	/* initialise the arguments of the structure */
	(*list)->data = NULL;
	(*list)->next = NULL;
	(*list)->previous = NULL;

	/* read the file "gridBlocs.txt" and create the list of blocs */
	FILE * fichier = NULL;
	int currentRead = 0;
	int widthCount = 1;
	int heightCount = 1;

	fichier = fopen(filepath, "r");

	if (fichier == NULL)	{
			printf("Error when the reading of the file.\n");
			exit(EXIT_FAILURE);
	}

	char * firstLine = (char*)malloc(sizeof(char));
	firstLine = fgets(firstLine, 512, fichier);

	float width = 0.;
	float height = 0.;

	/* we search the width */
	int parcoursFirstLine = 0;
	while (firstLine[parcoursFirstLine] != ' ') {
		width *= 10;
		width += ((float)firstLine[parcoursFirstLine] - 48);
		parcoursFirstLine++;
	}
	parcoursFirstLine++;
	/* we search the height */
	while (firstLine[parcoursFirstLine] != '\n') {
		height *= 10;
		height += ((float)firstLine[parcoursFirstLine] - 48);
		parcoursFirstLine++;	
	}

	while (currentRead != EOF) {
		fscanf(fichier, "%d", &currentRead);
		/*fprintf(stderr, "lu : %d \n",currentRead);*/
	
		Bloc * bloc = NULL;
		/* creation of the blocs, with different type */
		switch (currentRead) {
		
			case CLASSIQUE :
				/* theme Bambou */
				if (theme ==1) {
					bloc = AllocBloc(PositionXY(-2.2 * widthCount + width + 2.6, -2.2 * heightCount + height + 3.), currentRead, 2., "ressources/texture_bloc_classique-1.jpg");
				}
				/* theme Space */
				else {
					bloc = AllocBloc(PositionXY(-2.2 * widthCount + width + 2.6, -2.2 * heightCount + height + 3.), currentRead, 2., "ressources/texture_bloc_classique-2.jpg");
				}
				AddBlocToList(bloc, list);
				break;

			case BONUS_BAR_GROW_UP :
				bloc = AllocBloc(PositionXY(-2.2 * widthCount + width + 2.6, -2.2 * heightCount + height + 3.), currentRead, 2., "ressources/texture_bloc_bar_grow_up.jpg");
				AddBlocToList(bloc, list);
				break;
				
			case BONUS_BAR_NARROW :
				bloc = AllocBloc(PositionXY(-2.2 * widthCount + width + 2.6, -2.2 * heightCount + height + 3.), currentRead, 2., "ressources/texture_bloc_bar_narrow.jpg");
				AddBlocToList(bloc, list);
				break;
				
			case BONUS_LIFE_ADD :
				bloc = AllocBloc(PositionXY(-2.2 * widthCount + width + 2.6, -2.2 * heightCount + height + 3.), currentRead, 2., "ressources/texture_bloc_live_add.jpg");
				AddBlocToList(bloc, list);
				break;
				
			case BONUS_LIFE_MINUS :
				bloc = AllocBloc(PositionXY(-2.2 * widthCount + width + 2.6, -2.2 * heightCount + height + 3.), currentRead, 2., "ressources/texture_bloc_live_minus.jpg");
				AddBlocToList(bloc, list);
				break;
				
			case BONUS_SLOWER :
				bloc = AllocBloc(PositionXY(-2.2 * widthCount + width + 2.6, -2.2 * heightCount + height + 3.), currentRead, 2., "ressources/texture_bloc_slower.jpg");
				AddBlocToList(bloc, list);
				break;
				
			case BONUS_FASTER :
				bloc = AllocBloc(PositionXY(-2.2 * widthCount + width + 2.6, -2.2 * heightCount + height + 3.), currentRead, 2., "ressources/texture_bloc_faster.jpg");
				AddBlocToList(bloc, list);
				break;
				
			case INDESTRUCTIBLE :
				bloc = AllocBloc(PositionXY(-2.2 * widthCount + width + 2.6, -2.2 * heightCount + height + 3.), currentRead, 2., "ressources/texture_bloc_indestructible.jpg");
				AddBlocToList(bloc, list);
				break;
				
			case EMPTY :		
				break;
				
			default :
				break;
		}

		if (widthCount == width) {
			widthCount = 0;
			heightCount++;
		}
		widthCount++;
		currentRead = fgetc(fichier);
	}
	fclose(fichier);
}



ListBlocs DeleteOneBlocInTheList(ListBlocs * list) {
	if (list == NULL) {
		fprintf(stderr, "Pointer NULL for list in function DeleteOneBlocInTheList \n");
		exit(EXIT_FAILURE);
	}

	ListBlocs tmp = NULL;
	if ((*list) != NULL && (*list)->next != NULL) {
		tmp = (*list)->next;
	}
	/* if a previous and a next exist */
	if ((*list)->previous != NULL && (*list)->next != NULL) {
		((*list)->previous)->next = (*list)->next;
		((*list)->next)->previous = (*list)->previous;
	}
	/* if it's the last element */
	if ((*list)->previous != NULL && (*list)->next == NULL) {
		((*list)->previous)->next = NULL;
	}
	/* if it's the first element */
	if ((*list)->previous == NULL && (*list)->next != NULL) {
		((*list)->next)->previous = NULL;
	}
	free(*list);
	return tmp;
}



void DeleteListBlocs(ListBlocs * list) {
	if (list == NULL) {
		fprintf(stderr, "Pointer NULL for list in function DeleteListBlocs \n");
		exit(EXIT_FAILURE);
	}
	while(*list != NULL) {
		*list = DeleteOneBlocInTheList(list);
	}
}
