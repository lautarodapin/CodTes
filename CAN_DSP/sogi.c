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
//		Nombre: sogi.c																//
//                                                                                  //
//		Marcelo R. Curti															//
//																					//
//		16/03/2011																	//
//																					//
//##################################################################################//

#ifndef SOGI_C_
#define SOGI_C_
//
#include "sogi.h"

//##################################################################################//
//							        Funcion Sogi_1_bkbk								//
//##################################################################################//
//																					//
// 	Esta funcion permite obtener las componentes directa y en cuadratura de una 	//
// 	señal utilizando un DSOGI implementado con integradores backward-backward Euler.//
//																					//
void Sogi_1_bkbk(struct VECTOR_Sogi_bkbk *X, float periodo, float omega0, float K, float Gamma)
{
// -------- variables internas ---------- 
	float ev=0, ub1=0, ub1_ast=0, ub2_ast=0, u_fll=0;
// ------------- algoritmo --------------
	ev = X->vi - X->v ;
	ub1 = (ev * K - X->qv);
	ub1_ast = ub1 * (X->w * periodo);
	X->v += ub1_ast;
	//ub2 = yb1;
	ub2_ast = X->v * (X->w * periodo);
	X->qv += ub2_ast;
//----------------- FLL -----------------
	u_fll = X->qv * ev * Gamma * periodo;
	X->w_fll += u_fll;
	X->w = omega0 - X->w_fll;
		
}

//##################################################################################//
//							        Funcion Sogi_1_fdbk								//
//##################################################################################//
// 	Definicion prototipica de la funcion " Sogi_1_fdbk ".							//
//																					//
// 	Esta funcion permite obtener las componentes directa y en cuadratura de una 	//
// 	señal utilizando un DSOGI implementado con integradores forward-backward Euler.	//
//																					//
void Sogi_1_fdbk(struct VECTOR_Sogi_fdbk *X, float periodo, float omega0, float K, float Gamma)
{
// -------- variables internas ---------- 
	float ev=0, uf1_ast=0, ub2_ast=0, u_fll=0;
// ------------- algoritmo --------------
	uf1_ast = X->u * (X->w * periodo);
	X->v += uf1_ast;
	ub2_ast = X->v * (X->w * periodo);
	X->qv += ub2_ast;
	ev = X->vi - X->v;
	X->u = (ev * K - X->qv);
//----------------- FLL -----------------		
	u_fll = X->qv * ev * Gamma * periodo;
	X->w_fll += u_fll;
	X->w = omega0 - X->w_fll;
}

//##################################################################################//
//							        Funcion Sogi_3_bkbk								//
//##################################################################################//
// 	Definicion prototipica de la funcion " Sogi_3_bkbk ".							//
//																					//
// 	Esta funcion permite obtener las componentes alfa y beta de secuencia positiva 	//
//	fundamental de dos señales en descomposición alfa beta							//
// 	utilizando un DSOGI implementado con integradores backward-backward Euler.		//
//																					//
void Sogi_3_fdbk(struct VECTOR_Sogi_fdbk *Xalfa, struct VECTOR_Sogi_fdbk *Xbeta, float periodo, float omega0, float K, float Gamma)
{
	Xalfa->vi = Xalfa->vi * 0.5;
	Xbeta->vi = Xbeta->vi * 0.5;
	
	Sogi_1_fdbk(Xalfa, periodo, omega0, K, Gamma);
	
	Xbeta->w = Xalfa->w;			//	utiliza sólo el FLL de la fase alfa
	
	Sogi_1_fdbk(Xbeta, periodo, omega0, K, Gamma);
	
	Xalfa->v1 = Xalfa->v - Xbeta->qv;
	Xbeta->v1 = Xbeta->v + Xalfa->qv;
	
}

//##################################################################################//
//							        Funcion Sogi_3_fdbk								//
//##################################################################################//
// 	Definición prototipica de la función " Sogi_3_fdbk ".							//
//																					//
// 	Esta funcion permite obtener las componentes alfa y beta de secuencia positiva 	//
//	fundamental de dos señales en descomposición alfa beta							//
// 	utilizando un DSOGI implementado con integradores forward-backward Euler.		//
//																					//
void Sogi_3_bkbk(struct VECTOR_Sogi_bkbk *Xalfa, struct VECTOR_Sogi_bkbk *Xbeta, float periodo, float omega0, float K, float Gamma)
{
	Xalfa->vi = Xalfa->vi * 0.5;
	Xbeta->vi = Xbeta->vi * 0.5;
	
	Sogi_1_bkbk(Xalfa, periodo, omega0, K, Gamma);
	
	Xbeta->w = Xalfa->w;			//	utiliza sólo el FLL de la fase alfa
	
	Sogi_1_bkbk(Xbeta, periodo, omega0, K, Gamma);
	
	Xalfa->v1 = Xalfa->v - Xbeta->qv;
	Xbeta->v1 = Xbeta->v + Xalfa->qv;
}


#endif /*SOGI_C_*/
