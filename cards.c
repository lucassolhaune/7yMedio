#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Tamanio total del mazo de cartas.
const int CARDS_STACK_SIZE = 40;

/**
 * Esta funcion crea un array de enteros de 3 digitos utilizando el lugar de la centena para codificar el tipo de palo
 * (copa, oro, basto, espada) y las otras 2 posiciones restantes para el numero de la carta. Asi, el 101 corresponde
 * al 1 de copa, el 306 al 6 de basto y el 412 al 12 de espada.
 *
 * Luego de crear el arreglo, se le aplica un algoritmo de aleatorizacion de forma tal que cada vez que se llame a esta
 * funcion, las cartas nunca estaran en la misma posicion.
 *
 * @return array de 40 cartas mezcladas (ordenadas aleatoriamente).
 */
int *getCardsStack() {
	int *cardStack = (int*) malloc(CARDS_STACK_SIZE * sizeof(int));	//Reservo espacio para 40 cartas.
	int index = 0;
	int cardType, cardNumber;

	for (cardType = 1; cardType <= 4; ++cardType) {
		for (cardNumber = 1; cardNumber <= 12; ++cardNumber) {
			if (cardNumber == 8 || cardNumber == 9) {
				continue;	//Cuando encuentro el numero 8 o 9 de la carta, continuo con la interacion del for sin agregar estas cartas al stack.
			}

			//Armo el mazo de cartas con valores de 3 digitos. Centena para indicar palo y los 2 digitos restantes para el numero.
			cardStack[index] = (cardType * 100) + cardNumber;
			index++;
		}
	}

	/**
	 * Para mezclar las cartas, generamos dos valores aleatorios entre 0 y 39 y las posiciones del stack que correspondan a esos valores
	 * las intercambiamos entre si. Repetimos este proceso 100 veces para obtener un mazo de cartas bien mezclado.
	 *
	 * Para que cada vez que se ejecute el programa las cartas no nos queden ordenadas de la misma forma que la ejecucion
	 * anterior, llamamos a la funcion srand() de tal manera que cada vez que se tenga que mezclar, genere otro valor
	 * inicial (semilla). De esta forma, nos aseguramos que nunca se van a repetir los valores aleatorios por cada ejecucion del programa.
	 */
	srand(time(NULL));
	int j, r1, r2, temp;
	for (j = 1; j <= 100; j++) {
		r1 = rand() % 40;
		r2 = rand() % 40;
		temp = cardStack[r1];
		cardStack[r1] = cardStack[r2];
		cardStack[r2] = temp;
	}

	return cardStack;
}

/**
 * Recorremos el mazo de cartas desde la ultima posicion hasta la primera. Cuando encontramos una carta que no tenga un
 * 0 en su posicion, le asignamos un 0 a esa posicion y la devolvemos. De esta forma, la proxima vez que se llame a la
 * funcion, dicha carta no estara mas disponible.
 *
 * @param cardStack array de 40 cartas.
 * @return carta elegida.
 */
int popCard(int *cardStack) {
	size_t i;
	for (i = CARDS_STACK_SIZE - 1; i >= 0; i--) {
		if (cardStack[i] == 0) {
			continue;	//Cuando encontramos una posicion que contiene un 0 en el stack, continuamos sin devolver esa carta.
		}
		//Por otra parte, si encontramos una posicion que no tenga 0, le colocamos un 0 y devolvemos esa carta.
		int card = cardStack[i];
		cardStack[i] = 0;
		return card;
	}

	return 0;
}

/**
 * Para obtener el palo de la carta, dividimos el valor de 3 digitos por 100 para que nos quede un unico numero representando el mismo.
 *
 * Por ejemplo, si card = 312 ---> 312/100 = 3 ---> 3 representa el basto.
 *
 * @param card la carta de la cual queremos obtener su tipo.
 * @return el tipo de carta de forma numeral, por ej.: 1 representa Copa, 2 Oro, 3 Basto y 4 Espada.
 */
int getCardType(int card) {
	return (int) (card / 100);
}

/**
 * Para obtener el numero de la carta, le restamos el valor literal de la carta (ej. 402) al resultado de la multiplicacion del palo por 100.
 *
 * Por ejemplo: Si el valor de la carta en conjunto fuera 402 ---> 402 - (4 * 100) = 2 ---> 2 es el numero de la carta.
 *
 * @param card la carta de la cual queremos obtener su numero.
 * @return el numero de la carta.
 */
int getCardNumber(int card) {
	return (card - (getCardType(card) * 100));
}

/**
 * Para saber el nombre completo de la carta, tomo los valores que devuelven las funciones getCardType() y getCardNumber().
 *
 * Tras obtener el numero que representa cada palo, evaluo las diferentes condiciones y a cada numero, le asigno el nombre del palo.
 *
 * Luego realizo una concatenacion del numero de la carta junto a su palo utilizando la funcion snprintf() y guardando el mismo como string
 * en el buffer que le pasemos, en nuestro caso, denominado cardString. De esta manera, guardamos, por ejemplo, "7 de copa" en cardString y
 * lo devolvemos como nombre completo de la carta.
 *
 * @param card la carta de la cual queremos obtener su representacion literal.
 * @return una representacion literal de la carta, por ej.: 9 de Copa.
 */
char *getCardString(int card) {
	int cardNumber = getCardNumber(card);
	int cardType = getCardType(card);
	char *cardTypeString;
	switch (cardType) {
	case 1:
		cardTypeString = "Copa";
		break;
	case 2:
		cardTypeString = "Oro";
		break;
	case 3:
		cardTypeString = "Basto";
		break;
	case 4:
		cardTypeString = "Espada";
		break;
	default:
		printf("Error en la generacion de cartas: tipo no reconocido: %i", cardType);
		cardTypeString = "No reconocido";
	}

	/**
	 * 12 es el numero total que puede tener la cadena de texto mas larga, ej: 12 de Espada
	 * Pero con 12 habia casos donde se imprimia basura. Agregando uno mas se arreglo.
	 */
	size_t size = 13 * sizeof(char);
	char *cardString = (char*) malloc(size);

	//Concatenamos el nombre completo de la carta (numero + palo).
	snprintf(cardString, size, "%i de %s", cardNumber, cardTypeString);

	return cardString;
}

/**
 * Las cartas valen tantos puntos como su valor numerico indica, por ejemplo, el 4 de copa vale 4, el 7 de oro
 * vale 7, el 1 de basto vale 1. La excepcion a esto son las figuras (sota(10), caballo(11), rey(12)) que valen
 * medio punto.
 *
 * @param card la carta de la cual queremos obtener su valor.
 * @return 0.5 si la carta es 10, 11 o 12, de otra forma devuelve el valor intrinseco de la carta.
 */
double getCardPoints(int card) {
	int cardNumber = getCardNumber(card);
	if (cardNumber == 10 || cardNumber == 11 || cardNumber == 12) {
		return 0.5;
	}
	return cardNumber;
}
