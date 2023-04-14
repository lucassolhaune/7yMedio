#include <stdio.h>
#include <stdbool.h>

/**
 * Esta funcion evita que el usuario escriba un valor del tipo char en una variable del tipo integer.
 * Cada vez que el usuario escriba un dato de otro tipo que no sea integer, le mostrara un mensaje de error
 * en el que pedira nuevamente que se le ingrese datos.
 *
 * Nota:
 * Cuando el usuario escribia mas de una letra se mostraba el mensaje de error como tantas letras se escribia.
 * Esto se resolvio haciendo una limpieza de buffer, en el segundo while.
 *
 * Nota 2:
 * Conociendo de antemano la posibilidad de entrar en un loop infinito al usar while(true), lo evitamos
 * utilizando un break al final del while para que, en caso de que la condicion siga siendo verdadera salga
 * del bucle.
 *
 * @param userQuestion Pregunta al usuario sobre algun dato. Como por ejemplo, cantidad de jugadores.
 * @return Valor ingresado por el usuario.
 */
int getUserInputAsInt(char *userQuestion, int min, int max){
	int userInput, buffer;
	printf("\n");
	while (true) {
		printf("%s [%d - %d]: ", userQuestion, min, max);
		scanf("%i", &userInput);	//Guardamos la respuesta del usuario en userInput.
		while ((buffer =  getchar()) != '\n' ? buffer != EOF : false); //Realizamos una limpieza del buffer.

		//Si userInput no se encuentra entre los parametros correctos, enviamos un mensaje de error.
		if (userInput < min || userInput > max) {
			printf("\nEso no es correcto ! Intenta nuevamente - ");
		} else {
			break;
		}
	}
	return userInput;
}

double getUserInputAsDouble(char *userQuestion, double min, double max){
	int buffer;
	double userInput;
	printf("\n");
	while (true) {
		printf("%s [%.2f - %.2f]: ", userQuestion, min, max);
		scanf("%lf", &userInput);	//Guardamos la respuesta del usuario en userInput.
		while ((buffer =  getchar()) != '\n' ? buffer != EOF : false);	//Realizamos una limpieza del buffer.

		//Si userInput no se encuentra entre los parametros correctos, enviamos un mensaje de error.
		if (userInput < min || userInput > max) {
			printf("\nEso no es correcto ! Intenta nuevamente - ");
		} else {
			break;
		}
	}
	return userInput;
}

bool getUserInputAsBool(char *userQuestion) {
	char userInput;
	int buffer;
	printf("\n");
	while (true) {
		printf("%s [s/n]: ", userQuestion);
		scanf("%c", &userInput);	//Guardamos la respuesta del usuario en userInput.
		while ((buffer =  getchar()) != '\n' ? buffer != EOF : false);	//Realizamos una limpieza del buffer.

		//Si userInput no contiene el caracter requerido, enviamos un mensaje de error.
		if (userInput != 's' && userInput != 'S' && userInput != 'n' && userInput != 'N') {
			printf("\nEso no es correcto ! Intenta nuevamente - ");
		} else {
			break;
		}
	}
	return userInput == 's' || userInput == 'S' ;
}
