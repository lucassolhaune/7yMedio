//Trabajo realizado por Lucas Solhaune y Leandro Gomez.

#include "cards.h"
#include "inputs.h"
#include <stdio.h>
#include <stdbool.h>

int main() {
	const int MIN_PLAYERS = 1;	//Cantidad minima permitida de jugadores.
	const int MAX_PLAYERS = 5;	//Cantidad maxima permitida de jugadores.
	const int MIN_ROUNDS = 2;	//Cantidad minima permitida de rondas.
	const int MAX_ROUNDS = 4;	//Cantidad maxima permitida de rondas.
	const double MIN_BET = 100;	//Cantidad minima permitida de apuestas.
	const double MAX_BET = 1500;	//Cantidad maxima permitida de apuestas.

	int card, players, rounds, currentCard, playerNumber, profitWinner, cardNum, temp, max;
	double benchPoints, bet, maxBet, maxScore, totalBalance;
	bool option, secondCardPopped;

	int playersOut = 0;	//Contador para saber cuantos jugadores quedaron afuera en la partida.
	int sieteFigura = 0;	//Contador para saber cuantos jugadores lograron siete + figura.
	bool benchStatus = true;//Variable para definir el estado de la banca (ganadora o perdedora). Valor por default ganadora.
	double maxProfit = 0;	//Variable para comparar ganancias.
	double benchWallet = 100000;	//Monto con el que empieza la banca.

	printf("\n================================================================");
	printf("\n\t\tBienvenido al juego Siete y Medio");
	printf("\n================================================================\n");

	printf("\nPara comenzar, por favor ingrese los siguientes datos:");

	//Determinamos la cantidad de jugadores que habra en la partida.
	players = getUserInputAsInt("\nCantidad de jugadores con los que desee jugar", MIN_PLAYERS, MAX_PLAYERS );
	//Determinamos la cantidad de rondas que habra en la partida.
	rounds = getUserInputAsInt("Cantidad de rondas que dura la partida", MIN_ROUNDS, MAX_ROUNDS);

	double playerPoints[players];	//Array que contiene la cantidad de puntos de cada jugador.
	double playersWallet[players];	//Array donde se almacena el dinero de cada jugador.
	double playersBet[players];	//Array que contiene la apuesta que hace cada jugador por ronda.
	double playerGainRatio[players];//Array que contiene el porcentaje de ganancia sobre la apuesta.
	double playersBalance[players];	//Array donde se guarda el balance de cada jugador.
	double maxBetRecord[3];		//Array para guardar la mayor apuesta que se hizo en la partida, ronda y jugador.
	bool winners[players];		//Array que contiene los ganadores.
	int cardsCount[13];		//Array para contabilizar los numeros de las cartas que salieron a lo largo de la partida.
	int index[] = {0,1,2,3,4,5,6,7,8,9,10,11,12};	//Array que contiene los indices del array cardsCount[].

	//Le asignamos a cada jugador su monto inicial ($5000).
	for (playerNumber = 0; playerNumber < players; playerNumber++) {
		playersWallet[playerNumber] = 5000;
	}

	//Inicializamos todos los valores del array cardsCount[] en 0.
	for (int i = 0; i < 13; i++) {
		cardsCount[i] = 0;
	}

	//Inicializamos en 0 el array maxBetRecord[]
	for (int i = 0; i < players; i++) {
		maxBetRecord[i] = 0;
	}

	//Inicia la ronda.
	for (int roundNumber = 1; roundNumber <= rounds; roundNumber++) {

		//Genero el mazo de cartas mezclado aleatoriamente.
		int *cardStack = getCardsStack();

		printf("\n---------------------------------------------------------------");
		printf("\n\t\t\tRonda %d", roundNumber);
		printf("\n---------------------------------------------------------------\n");

		//Inicializamos el array de ganadores en false.
		for (playerNumber = 0; playerNumber < players; playerNumber++) {
			winners[playerNumber] = false;
		}

		//Turno de cada uno de los jugadores en la ronda.
		for (playerNumber = 0; playerNumber < players ; playerNumber++) {

			//Elijo una carta aleatoria y en su posicion dentro del array lo reemplazo por un 0 para que no vuelva a aparecer en la ronda.
			card = popCard(cardStack);
			playerPoints[playerNumber] = getCardPoints(card);

			/**
			 * Para saber que numero de carta se repite mas, tomo el valor numerico de la carta que sale y a esa carta
			 * le sumo 1 dentro del array cardsCount en su posicion.
			 *
			 * Nota: las posiciones 7 y 8 del array van a ser siempre 0 ya que no existen las cartas 8 y 9.
			 */
			cardNum = getCardNumber(card);
			cardsCount[cardNum] = cardsCount[cardNum] + 1;



			//Si a un jugador le queda menos dinero que el minimo que se puede apostar, no puede jugar mas.
			if (playersWallet[playerNumber] < MIN_BET) {
				printf("Usted no posee saldo suficiente para apostar y en consecuencia no puede seguir jugando.");
				continue;
			}

			printf("\nTurno del jugador %d", playerNumber + 1);
			printf("\n\tCarta recibida: %s", getCardString(card));

			/**
			 * Elegimos el minimo valor entre el maximo permitido de apuesta (1500) y el saldo en la billetera del jugador.
			 * El objetivo es mostrar en pantalla el maximo limite de apuesta. Es decir, si el jugador puede apostar menos
			 * de 1500, se mostrara lo maximo que el jugador pueda apostar.
			 *
			 * Para ello, utilizamos lo que se conoce como operador ternario donde primero evaluamos la condicion y si es verdadera,
			 * ejecuta la instruccion del primer statement (lo que se encuentra junto a '?'). Si la condicion es falsa,
			 * aplica el segundo statement (junto a ':'). Lo desarrollamos de esta manera porque es mucho mas sencillo que hacer lo mismo
			 * con un if.
			 */
			maxBet = MAX_BET <= playersWallet[playerNumber]
					? MAX_BET
					: playersWallet[playerNumber];
			/**
			 * Llamamos a la funcion getUserInputAsDouble() para que no pueda ingresar un valor que no sea numerico.
			 * Si ingresa un caracter que no sea numerico, la funcion lanza un mensaje de error y vuelve a preguntar por un valor correcto.
			 */
			bet = getUserInputAsDouble("\tIngrese su apuesta $", MIN_BET, maxBet);
			playersBet[playerNumber] = bet;

			/**
			 * Guardamos record de apuestas.
			 * Nota: Solo guardamos un solo valor, es decir, si se repite la apuesta maxima solo se guarda el primero que la logre.
			 */
			if (bet > maxBetRecord[0]) {
				maxBetRecord[0] = bet;			//Posicion 0 del array almacena apuesta max.
				maxBetRecord[1] = playerNumber + 1;	//Posicion 1 del array almacena jugador que logro esa apuesta.
				maxBetRecord[2] = roundNumber;		//Posicion 2 del array almacena ronda donde se logro esa apuesta.
			}
			//Le restamos a la billetera del jugador el monto apostado, y mostramos su saldo.
			playersWallet[playerNumber] = playersWallet[playerNumber] - bet;
			printf("\t(Saldo %.2f)", playersWallet[playerNumber]);
			printf("\t\tPuntaje de la carta: %.2f", playerPoints[playerNumber]);

			secondCardPopped = true;	//Valor por default de segunda carta mostrada.
			playerGainRatio[playerNumber] = 1; // Valor por default en caso de que no gane.

			//Preguntamos si quiere otra carta y evaluamos su puntaje total.
			do {
				/**
				 * Llamamos a la funcion getUserInputAsBool() para que no pueda ingresar un caracter invalido.
				 * Si ingresa un caracter que no sea 's' para afirmar o 'n' para negar, la funcion lanza un mensaje de
				 * error y vuelve a preguntar por un valor correcto.
				 */
				option = getUserInputAsBool("\n\tDesea pedir otra carta?");
				if (option) {
					currentCard = popCard(cardStack);	//Recibo la segunda carta.
					playerPoints[playerNumber] += getCardPoints(currentCard);	//Tomo su puntaje.
					printf("\tCarta recibida: %s", getCardString(currentCard));	//Muestro la segunda carta.

					//Realizo conteo con la carta que salio.
					cardNum = getCardNumber(currentCard);
					cardsCount[cardNum] = cardsCount[cardNum] + 1;

					//Analizo si llego a 7.5 con solo dos cartas.
					if (secondCardPopped) {
						secondCardPopped = false;
						//Guardo el palo y numero de ambas cartas para evaluar, en caso de que llegue a 7.5, el porcentaje de ganancia que le corresponde.
						int cardType1 = getCardType(card);
						int cardType2 = getCardType(currentCard);
						int cardNum1 = getCardNumber(card);
						int cardNum2 = getCardNumber(currentCard);

						if (playerPoints[playerNumber] == 7.5) {
							if ((cardType1 == 2 && cardType2 == 2) && (cardNum1 == 12 || cardNum2 == 12)) {
								playerGainRatio[playerNumber] = 2;	//Si gana con: 7 + figura (palo oro y figura rey), premio 100% de lo apostado.
							} else if (cardType1 == cardType2) {
								playerGainRatio[playerNumber] = 1.75;	//Si gana con: 7 + figura, del mismo palo, premio 75% de lo apostado
							} else {
								playerGainRatio[playerNumber] = 1.50;	//Si gana con: 7 + figura, el premio es 50% de lo apostado
							}
							printf("\nHa alcanzado 7.5 puntos con 2 cartas.\n");
							sieteFigura ++;	//Contador para saber cuantos jugadores logran 7.5 + figura, es decir, con 2 cartas.
							break;
						}
					}

					//Analizo si llego a 7.5 con tres cartas.
					if (playerPoints[playerNumber] == 7.5){
						playerGainRatio[playerNumber] = 1.25;
						printf("\nHa alcanzado 7.5 puntos con 3 o mas cartas.\n");
						break;
					}

					//Si se pasa de 7.5 pierde la apuesta.
					if (playerPoints[playerNumber] > 7.5) {
						printf("\nUsted pierde la ronda. Ha superado 7.5. Total de puntos: %.2f\n", playerPoints[playerNumber]);
						playersOut ++;	//Contador para saber cuantos jugadores se pasaron de 7.5.
						break;
					}
					printf ("\tJugador %d tiene %.2f puntos", playerNumber + 1, playerPoints[playerNumber]);	//Muestro el puntaje del jugador.
				}
			} while (option);
		}

		//Turno de la banca.
		printf("\nTurno de la banca");
		card = popCard(cardStack);	//Saco una carta.
		cardNum = getCardNumber(card);	//Tomo su valor numerico y lo sumo al contador de cartas.
		cardsCount[cardNum] = cardsCount[cardNum] + 1;

		benchPoints = getCardPoints(card);	//Sumo el puntaje de la carta al puntaje de la banca.
		printf("\n\tLa banca recibe el %s", getCardString(card));

		//La banca pide otra carta.
		while (benchPoints <= 5.5) {	//Mientras el puntaje de la banca sea menor o igual a 5.5, pide otra carta.
			card = popCard(cardStack);	//Saco otra carta.
			printf("\n\tLa banca pide otra carta y recibe el %s. ", getCardString(card));
			benchPoints += getCardPoints(card);	//Sumo el puntaje de la carta a la banca.
			printf("Puntos acumulados: %.2f", benchPoints);
			cardNum = getCardNumber(card);
			cardsCount[cardNum] = cardsCount[cardNum] + 1;	//Realizo conteo con la nueva carta
		}
		//La banca gana.
		if (benchPoints == 7.5) {	//Si la banca gana con 7.5, todos los jugadores pierden sus apuestas independientemente de sus respectivos puntajes.

			printf("\nLa banca gana con 7.5 puntos. Todos los jugadores pierden sus apuestas.\n");
			for(int i = 0; i < players; i++) {
				benchWallet += playersBet[i];	//La apuesta de cada jugador la sumo al tesoro de la banca.
			}
			benchStatus = true;	//Como la banca es ganadora, benchStatus queda en true.
		}
		//La banca se pasa.
		if (benchPoints > 7.5) {	//Si la banca se pasa 7.5, pierde y ganan los jugadores.
			printf("\nLa banca pierde: ha superado 7.5 puntos. Ganan los jugadores!\n");
			for (int i = 0; i < players; i++) {
				if (playerPoints[i] == 7.5) {	//Si algun/os jugador/es llega a 7.5, le devuelvo la apuesta sumado al ratio de ganancia que tuvo.
					playersWallet[i] += playersBet[i] * playerGainRatio[i];
					benchWallet = benchWallet - (playersBet[i] * (playerGainRatio[i] - 1));	//A su vez, ese ratio se lo resto al tesoro de la banca.
				} else {	//Si se pasa la banca, ganan todos los jugadores, por ende, aunque los jugadores se hayan pasado, todos recuperan lo apostado.
					playersWallet[i] += playersBet[i];
				}

				winners[i] = true;	//En este caso, al ser todos los jugadores ganadores, winners[] queda en todas sus posiciones como true.
			}
			benchStatus = false;	//La banca, por otro lado, queda en estado false, es decir, perdedora.
		}
		//La banca se planta.
		if (benchPoints >= 6 && benchPoints < 7.5) {	//Si la banca tiene entre 6 y 7.5 puntos, se planta y se comparan los puntajes con los jugadores en juego.
			printf("\nLa banca se planta. Ha acumulado %.2f puntos.\n", benchPoints);
			maxScore = 0;
			/**
			 * La comparacion la hacemos recorriendo el array de puntajes y guardando la mayor puntuacion en la variable maxScore.
			 *
			 * A medida que recorremos el array de puntajes, comparamos si el siguiente puntaje es mayor al que estaba almacenado en maxScore y, si es asi, reemplazamos el valor de la variable
			 * por el nuevo puntaje mas alto. A su vez, mientras vamos encontrando mayores puntuaciones, le asignamos en el array winners[] la condicion de true al jugador que tenga
			 * el puntaje mas alto. Si encontramos otro puntaje mas alto a ese, limpiamos el array de winners[] y colocamos true unicamente a la posicion del jugador que supero la puntuacion.
			 *
			 * Por otro lado, si encontramos un puntaje igual al mas alto, le otorgamos la condicion true a ambos jugadores para indicar que los dos son ganadores.
			 */
			for (int i = 0; i < players; i++){
				if ((playerPoints[i] <= 7.5) && (playerPoints[i] > benchPoints)) {	//Analizamos si el jugador no se paso y si tiene un puntaje mayor al de la banca.
					if (playerPoints[i] > maxScore) {	//Si se cumple la condicion anterior, comparamos ese puntaje contra maxScore.
						maxScore = playerPoints[i];
						for (int j = 0; j < players; j++) {
							winners[j] = false;	//Limpiamos el array de winners[].
						}
						winners[i] = true;	//Si es mayor, colocamos true unicamente a ese jugador.
					} else if (playerPoints[i] == maxScore) {
						winners[i] = true;	//Si es igual, los jugadores que tengan el mismo puntaje que maxScore tienen estado true.
					}
					benchStatus = false; //Si hay al menos un jugador ganador, pierde la banca.
				}
			}
			/**
			 * Luego de que tenemos los ganadores definidos, calculamos el monto que se lleva cada uno.
			 *
			 * Si uno de esos ganadores gana con 7.5, se lleva lo apostado mas el porcentaje de ganancia que le corresponda.
			 * Por otra parte, si solo gana por tener puntuacion mas alta sin necesariamente haber conseguido 7.5, se le devuelve el valor de lo apostado.
			 */
			for(int i = 0; i < players; i++) {
				if (winners[i] == true) {
					if (playerPoints[i] == 7.5) {
						playersWallet[i] += playersBet[i] * playerGainRatio[i];
						benchWallet = benchWallet - (playersBet[i] * (playerGainRatio[i] - 1));
					} else {
						playersWallet[i] += playersBet[i];
					}
				} else {
					benchWallet += playersBet[i];
				}
			}
		}

		//Muestro estadisticas al finalizar cada ronda y vacio array de apuestas.
		printf("\nFin de la ronda %d.\nEstadisticas:", roundNumber);
		if (benchStatus == true) {	//Si benchStatus es true, significa que gano la banca.
			printf("\n\t-Gano la banca. %.2f puntos. Tesoro: $%.2f", benchPoints, benchWallet);
		} else {
			printf("\n\t-Perdio la banca. %.2f puntos. Tesoro: $%.2f", benchPoints, benchWallet);
		}
		for(int i = 0; i < players; i++) {
			if (winners[i] == true) {	//Todos los jugadores que en el array winners[] sean true, son ganadores.
				printf("\n\t-Jugador %d gano. Puntos: %.2f. Aposto: %.2f. Saldo: $%.2f", i + 1, playerPoints[i], playersBet[i], playersWallet[i]);
			} else {
				printf("\n\t-Jugador %d perdio. Puntos: %.2f. Aposto: %.2f. Saldo: $%.2f", i + 1, playerPoints[i], playersBet[i], playersWallet[i]);
			}
			playersBet[i] = 0;	//Vacio array de apuestas.
		}

		benchStatus = true;	//Reinicio valor por default del estado de la banca.

	}
	//Termina la ronda.

	/**
	 * Para saber que jugador fue el que mas ganancias tuvo y el balance general de todos los jugadores:
	 *
	 * 1) Calculamos el balance general realizando la diferencia entre el monto actual y el monto inicial de cada jugador.
	 *
	 * 2) Conocemos el jugador que mas dinero gano, comparando todas las diferencias de balances entre ellos y guardando el mayor
	 * en la variable maxProfit. Luego, guardamos el numero de jugador que logro tener mayor ganancia en la variable profitWinner.
	 */
	for(int i = 0; i < players; i++) {
		totalBalance = totalBalance + (playersWallet[i] - 5000);
		if (totalBalance != 0) {
			if ((playersWallet[i] - 5000) > maxProfit) {
				maxProfit = playersWallet[i] - 5000;
				profitWinner = i;
			}
		}
	}

	/**
	 * Algoritmo para definir el top 5 de cartas que mas salieron:
	 *
	 * Recorro el array cardsCount[] y si encuentro un valor mayor en la posicion siguiente, lo guardo en max para luego
	 * hacer un swap de posiciones, tanto en el array cardsCount como el indice de esa posicion, para no perder el numero de la carta.
	 *
	 * Repetimos el proceso como tantas cartas queremos ordenar. En nuestro caso, 5 veces.
	 */
	for (int i = 1; i <= 5; i++) {
		max = i;
		for (int j = i + 1; j < 13; j++) {
			if (cardsCount[j] > cardsCount[max]) {
				max = j;	//Si en cardsCount[i+1] encontramos un valor mayor a cardsCount[i], le asignamos a max ese nuevo valor.
			}
		}
		//Hacemos un swap de posiciones de cardsCount[] luego de encontrar la primer carta que mas salio.
		temp = cardsCount[i];
		cardsCount[i] = cardsCount[max];
		cardsCount[max] = temp;

		//Hacemos un swap de indices junto a la carta que cambiamos antes, para no perder su valor numerico.
		temp = index[i];
		index[i] = index[max];
		index[max] = temp;
	}


	//Muestro estadisticas en toda la partida.
	printf("\n---------------------------------------------------------------");
	printf("\nFin de la partida.\nEstadisticas:");

	//Mostramos cuantos jugadores lograron 7.5 con 2 cartas.
	printf("\n\t-Jugadores que lograron 7.5 + figura: %d", sieteFigura);

	//Mostramos los jugadores que se pasaron de 7.5.
	printf("\n\t-Jugadores que perdieron por pasarse de 7.5: %d", playersOut);

	//Mostramos el participante (sea jugador o banca) que mas dinero gano a lo largo de la partida.
	if (totalBalance != 0) {
		if ((benchWallet - 100000) > maxProfit) {	//Comparamos si la ganancia de la banca es mayor a la del jugador que mas gano.
			printf("\n\t-La banca gano mas dinero con un total de $%.2f en ganancias.", (benchWallet - 100000));
		} else {
			printf("\n\t-El jugador %d gano mas dinero con un total de $%.2f en ganancias.", (profitWinner + 1), maxProfit);
		}
	} else {
		printf("\n\t-No hubo ganadores en esta partida.");
	}

	//Mostramos el balance general de los jugadores.
	printf("\n\t-Balance general del conjunto de jugadores: $%.2f. ", totalBalance);
	if (totalBalance < 0) {	//Si el balance general es un numero negativo, significa que hubo mas perdidas que ganancias.
		printf("Fueron mas perdidas que ganancias.");
	} else if (totalBalance > 0) {
		printf("Fueron mas ganancias que perdidas.");
	} else {
		printf("No hubo ganancias en esta partida.");
	}
	//Mostramos la apuesta maxima realizada junto con su jugador y ronda.
	printf("\n\t-La apuesta maxima la realizo el jugador %0.f en la ronda %0.f con un monto total de $%2.f.", maxBetRecord[1], maxBetRecord[2], maxBetRecord[0]);

	//Mostramos el top 5 de los numeros de cartas que mas salieron en la partida.
	printf("\n\t-El top 5 de los numeros de las cartas que mas salieron son:\n");
	for (int i = 1; i <= 5; i++) {
		if (cardsCount[i] > 1) {
			printf("\t\t%d - Carta %d: %d veces.\n", i, index[i], cardsCount[i]);
		} else {
			printf("\t\t%d - Carta %d: %d vez.\n", i, index[i], cardsCount[i]);
		}
	}
	return 0;
}
