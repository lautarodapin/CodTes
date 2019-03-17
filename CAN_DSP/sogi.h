//##################################################################################//
//																					//        
//		Esta libreria realiza el calculo de detectores de secuencia positiva		//
//		monofásicos y trifásicos basados en integradores generalizados de segundo	// 
//		orden discretos	(Discrete Second Order Generalized Integrator, DSOGI).		//
//		Se implementan algoritmos de integración del tipo forward-backward Euler	//
//		y backward-backward Euler.													//
//		Los DSOGI cuentan además con un lazo de enganche de frecuencia (Frequency	//
//		Locked Loop, FLL).															//
//		Los algoritmos discretos de los DSOGI monofásicos se basan en el trabajo de	//
//		Rodriguez et. all, "Discrete-time implementation of SOGI for Grid Converters"
//		IEEE, 2008.																	//
//																					//
//		Nombre: sogi.h																//
//                                                                                  //
//		Marcelo R. Curti															//
//																					//
//		16/03/2011																	//
//																					//
//##################################################################################//

#ifndef SOGI_H_
#define SOGI_H_

//##################################################################################//
//		        	        Definicion de Estructuras							    //
//##################################################################################//


// -------- Estructura que interviene en un DSOGI del tipo backward-backward ---------

struct VECTOR_Sogi_bkbk {
						float vi;		// tensión de entrada
						float v;		// componente directa 
						float v1;		// componente directa de sec. positiva fund.
						float qv;		// componente en cuadratura
						float w;		// frecuencia
						float w_fll;	// variable interna del FLL
						};

// -------- Estructura que interviene en un DSOGI del tipo forward-backward ----------
				
struct VECTOR_Sogi_fdbk {
						float vi;		// tensión de entrada
						float v;		// componente directa 
						float v1;		// componente directa de sec. positiva fund.
						float qv;		// componente en cuadratura
						float w;		// frecuencia
						float w_fll;	// variable interna del FLL
						float u;		// variable interna del SOGI
						};

//##################################################################################//
//							        Funcion Sogi_1_bkbk								//
//##################################################################################//
// 	Definicion prototipica de la funcion " Sogi_1_bkbk ".							//
//																					//
// 	Esta funcion permite obtener las componentes directa y en cuadratura de una 	//
// 	señal utilizando un DSOGI implementado con integradores backward-backward Euler.//
//																					//
//	Los datos a cargar en la función son:											//
// 	- VECTOR de variables (del tipo VECTOR_Sogi_bkbk)								//
// 	- Período de muestreo en segundos												//
//	- frecuencia inicial para el FLL en rad/seg.									//
//	- ganancia del error del DSOGI 													//
//	- ganancia del FLL																//
//																					//
void Sogi_1_bkbk(struct VECTOR_Sogi_bkbk *X, float periodo, float omega0, float k, float Gamma);


//##################################################################################//
//							        Funcion Sogi_1_fdbk								//
//##################################################################################//
// 	Definicion prototipica de la funcion " Sogi_1_fdbk ".							//
//																					//
// 	Esta funcion permite obtener las componentes directa y en cuadratura de una 	//
// 	señal utilizando un DSOGI implementado con integradores forward-backward Euler.	//
//																					//
//	Los datos a cargar en la función son:											//
// 	- VECTOR de variables (del tipo VECTOR_Sogi_fdbk)								//
// 	- Período de muestreo en segundos												//
//	- frecuencia inicial para el FLL en rad/seg.									//
//	- ganancia del error del DSOGI 													//
//	- ganancia del FLL																//
//																					//
void Sogi_1_fdbk(struct VECTOR_Sogi_fdbk *X, float periodo, float omega0, float k, float Gamma);


//##################################################################################//
//							        Funcion Sogi_3_bkbk								//
//##################################################################################//
// 	Definicion prototipica de la funcion " Sogi_3_bkbk ".							//
//																					//
// 	Esta funcion permite obtener las componentes alfa y beta de secuencia positiva 	//
//	fundamental de dos señales en descomposición alfa beta							//
// 	utilizando un DSOGI implementado con integradores backward-backward Euler.		//
//																					//
//	Los datos a cargar en la función son:											//
// 	- VECTOR de variables alfa (del tipo VECTOR_Sogi_bkbk)							//
// 	- VECTOR de variables beta (del tipo VECTOR_Sogi_bkbk)							//
// 	- Período de muestreo en segundos												//
//	- frecuencia inicial para el FLL en rad/seg.									//
//	- ganancia del error del DSOGI 													//
//	- ganancia del FLL																//
//																					//
void Sogi_3_fdbk(struct VECTOR_Sogi_fdbk *Xalfa, struct VECTOR_Sogi_fdbk *Xbeta, float periodo, float omega0, float k, float Gamma);


//##################################################################################//
//							        Funcion Sogi_3_fdbk								//
//##################################################################################//
// 	Definicion prototipica de la funcion " Sogi_3_fdbk ".							//
//																					//
// 	Esta funcion permite obtener las componentes alfa y beta de secuencia positiva 	//
//	fundamental de dos señales en descomposición alfa beta							//
// 	utilizando un DSOGI implementado con integradores forward-backward Euler.		//
//																					//
//	Los datos a cargar en la función son:											//
// 	- VECTOR de variables alfa (del tipo VECTOR_Sogi_bkbk)							//
// 	- VECTOR de variables beta (del tipo VECTOR_Sogi_bkbk)							//
// 	- Período de muestreo en segundos												//
//	- frecuencia inicial para el FLL en rad/seg.									//
//	- ganancia del error del DSOGI 													//
//	- ganancia del FLL																//
//																					//
void Sogi_3_bkbk(struct VECTOR_Sogi_bkbk *Xalfa, struct VECTOR_Sogi_bkbk *Xbeta, float periodo, float omega0, float K, float Gamma);


#endif /*SOGI_H_*/
