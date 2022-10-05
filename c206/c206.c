/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	list->activeElement = NULL; //odstranění odkazu
	list->lastElement = NULL; //odstranění odkazu

	for(DLLElementPtr temp; list->firstElement; list->firstElement = temp){ //smazání všech prvků seznamu
		temp = list->firstElement->nextElement;
		free(list->firstElement);
	}
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr temp = malloc(sizeof(struct DLLElement)); //allokace paměti
	if(!temp){ //kontrola allokace paměti
		DLL_Error();
		return;
	}

	temp->data = data; //přiřazení dat novému prvku

	temp->previousElement = NULL; //předchozí prvek prvního prvku neexistuje => pointer bude NULL
	temp->nextElement = list->firstElement; //nový první prvek bude ukazovat na bývalý první prvek

	if(!list->firstElement){ //seznam je prázdný
		list->lastElement = temp; //první prvek je i posledním prvkem 
	}
	else{ //seznam není prázdný
		list->firstElement->previousElement = temp; // bývalý první prvek musí odkazovat na nový první prvek
	}

	list->firstElement = temp; //přiřazení odkazu na nový první prvek
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	if(list->firstElement != list->lastElement && !list->lastElement){ //kontrola podmínek dle zadání
		DLL_Error();
		return;
	}

	DLLElementPtr temp = malloc(sizeof(struct DLLElement)); //allokace paměti
	if(!temp){ //kontrola allokace paměti
		DLL_Error();
		return;
	}

	temp->data = data;//přiřazení dat novému prvku

	if(!list->lastElement){ //seznam je prázdný
		list->firstElement = temp;
		temp->previousElement = NULL;
		temp->nextElement = NULL;
	}
	else{ //seznam není prázdný
		temp->previousElement = list->lastElement;
		temp->nextElement = NULL;
		list->lastElement->nextElement = temp;
	}

	list->lastElement = temp; //nastavení ukazatele na konec seznamu
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if(!list->firstElement){ //ověření podmínky ze zadání
		DLL_Error();
		return;
	}
	*dataPtr = list->firstElement->data; //kopírování dat
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if(!list->lastElement){ //ověření podmínky ze zadání
		DLL_Error();
		return;
	}
	*dataPtr = list->lastElement->data; //kopírování dat
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	DLLElementPtr temp = list->firstElement; //dočasné uložení odkazu na první prvek

	if(!temp){ //ověření, jestli seznam není prázdný
		return;
	}

	if(temp == list->activeElement){ //první prvek je aktivní
		list->activeElement = NULL; //ruší se aktivita
	}

	if(temp == list->lastElement){ //seznam obsahuje jen jeden prvek
		list->lastElement = NULL;	//ruší se odkaz na poslední prvek
	}

	list->firstElement = list->firstElement->nextElement; //přesun ukazatele počátku seznamu na nový počátek

	free(temp); //uvolnění paměti
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	DLLElementPtr temp = list->lastElement; //dočasné uložení odkazu na první prvek

	if(!temp){ //ověření, jestli seznam není prázdný
		return;
	}

	if(temp == list->activeElement){ //poslední prvek je aktivní
		list->activeElement = NULL; //ruší se aktivita
	}

	if(temp == list->firstElement){ //seznam obsahuje jen jeden prvek
		//ruší se odkaz na poslední a první prvek 
		list->lastElement = NULL; 
		list->firstElement = NULL;
	}
	else{
		//nastavení nového konce seznamu
		list->lastElement->previousElement->nextElement = NULL; 
		list->lastElement = list->lastElement->previousElement; 
	}

	free(temp); //uvolnění paměti
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if(!list->activeElement || list->activeElement == list->lastElement){ //ověření podmínek ze zadání
		return;
	}

	DLLElementPtr temp = list->activeElement->nextElement; //dočasný ukazatel na mazaný prvek

	if(list->activeElement->nextElement->nextElement){ //mazaný prvek není posledním prvkem
		list->activeElement->nextElement->nextElement->previousElement = list->activeElement;
	}
	else{ //mazaný prvek je posledním prvek
		list->lastElement = list->activeElement;
		}

	list->activeElement->nextElement = list->activeElement->nextElement->nextElement; //upravení ukazatele na nový následující prvek

	free(temp); //uvolnění paměti
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if(!list->activeElement || list->activeElement == list->firstElement){ //ověření podmínek ze zadání
		return;
	}
	
	DLLElementPtr temp = list->activeElement->previousElement; //dočasný ukazatel na mazaný prvek

	if(list->activeElement->previousElement->previousElement){ //mazaný prvek není prvním prvkem
		//"přemostění" mazaného prvku
		list->activeElement->previousElement->previousElement->nextElement = list->activeElement; 
		list->activeElement->previousElement = list->activeElement->previousElement->previousElement;
	}
	else{ //mazaný prvek je prvním prvkem
		list->firstElement = list->activeElement; //přesun ukazetele na nový první prvek
		list->activeElement->previousElement = NULL; //nový prvek nemá předřazený prvek
	}

	free(temp); //uvolnění paměti
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if(!list->activeElement){ //seznam není aktivní
		return;
	}

	DLLElementPtr temp = malloc(sizeof(struct DLLElement)); //allokace paměti
	if(!temp){ //kontrola allokace paměti
		DLL_Error();
		return;
	}

	temp->data = data; //přiřazení dat novému prvku

	temp->nextElement = list->activeElement->nextElement; //nový následující prvek odkazuje na bývalý následující prvek
	temp->previousElement = list->activeElement; //nový následující prvek odkazuje zpět na aktivní prvek

	if(list->activeElement->nextElement){ //aktivní prvek má za sebou další prvek
		list->activeElement->nextElement->previousElement = temp; //navázání zpětného ukazatele z původního následujícího prvku na nový následující prvek 
	}
	else{
		list->lastElement = temp; //prvek se stává koncem seznamu
	}

	list->activeElement->nextElement = temp; //navázání za aktivní prvek
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if(!list->activeElement){ //kontrola aktivity
		return;
	}

	DLLElementPtr temp = malloc(sizeof(struct DLLElement)); //allokace paměti
	if(!temp){ //kontrola allokace paměti
		DLL_Error();
		return;
	}

	temp->data = data; //přiřazení dat novému prvku

	//nastavení ukazatelů nového prvku na prvek předchozí a následující
	temp->nextElement = list->activeElement;
	temp->previousElement = list->activeElement->previousElement;
	

	if(list->activeElement != list->firstElement){ //nový prvek není prvním prvkem seznamu
		list->activeElement->previousElement->nextElement = temp; //prvek před aktivním nově ukazuje na nový prvek
	}
	else{ //nový prvek není prvním prvkem seznamu
		list->firstElement = temp; //nastavení nového prvního prvku
	}

	list->activeElement->previousElement = temp; //zpětný ukazatel aktivního prvku ukazuje na nový prvek
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if(!list->activeElement){ //ověření aktivity
		DLL_Error();
		return;
	}

	*dataPtr = list->activeElement->data; //kopírování dat
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if(!list->activeElement){ //ověření aktivity
		return;
	}

	list->activeElement->data = data; //přepis dat
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if(!list->activeElement){ //ověření aktivity
		return;
	}

	list->activeElement = list->activeElement->nextElement; //přesun ukazatele
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if(!list->activeElement){ //ověření aktivity
		return;
	}

	list->activeElement = list->activeElement->previousElement; //přesun ukazatele
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement ? 1 : 0; //vrací 0, pokud je adresa NULL, v opačném případě vrací 1
}

/* Konec c206.c */
