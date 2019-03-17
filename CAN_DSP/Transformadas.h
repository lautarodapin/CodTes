//###########################################################################################//
//                                                                                           //        
//         Esta libreria realiza el calculo de las transformadas de Park y Clarke            //
//         directa e inversa invariantes en amplitud y potencia.                             //
//         Los ejes del referencial son considerados como: eje d es el eje directo           //                           
//         y el eje q es el eje en cuadratura que esta atrasado 90º respecto al eje          //
//         d. El eje d es el que se pega al vector Va.                                       //
//                                                                                           //
//         Nombre: Transformadas.h                                                           //
//                                                                                           //
//         Federico M. Serra                                                                 //
//                                                                                           //
//         17/11/2010                                                                        //
//                                                                                           //
//###########################################################################################//

#ifndef Transformadas_
#define Transformadas_

//###########################################################################################//
//				                Definicion de Estructuras								     //
//###########################################################################################//

// Estructura del vector que interviene en la transformacion de Park

struct VECTOR_Park{
			       float Xa;                                                  // Vector eje "a"
			       float Xb;                                                  // Vector eje "b"
			       float Xc;                                                  // Vector eje "c"
			       float Xd;                                                  // Vector eje "d"
			       float Xq;                                                  // Vector eje "q"
			       float X0;                                                  // Vector eje "0"
                  };

// Estructura del vector que interviene en la transformacion de Clarke

struct VECTOR_Clarke{
			       float Xa;                                                  // Vector eje "a"
			       float Xb;                                                  // Vector eje "b"
			       float Xc;                                                  // Vector eje "c"
			       float Xalpha;                                          // Vector eje "alpha"
			       float Xbeta;                                            // Vector eje "beta"
			       float X0;                                                  // Vector eje "0"
                  };


// Typedef para los nuevos tipos
typedef struct VECTOR_Park   vpark_t;
typedef struct VECTOR_Clarke vclarke_t;

//###########################################################################################//
//						        Funcion Transf_Park_Directa_IA 						         //
//###########################################################################################//

// Definicion prototipica de la funcion " Transf_Park_Directa_IA ".

// Esta funcion permite calcular la transformada directa de Park invariante en amplitud.
// Los datos a cargar en la funcion son:
// void Transf_Park_Directa_IA(Vector a Transformar, cos ang referencial, sin ang referencial)

   void Transf_Park_Directa_IA(struct VECTOR_Park *, float, float);

//###########################################################################################//
//						         Funcion Transf_Park_Inversa_IA						         //
//###########################################################################################//

// Definicion prototipica de la funcion " Transf_Park_Inversa_IA ".

// Esta funcion permite calcular la transformada inversa de Park invariante en amplitud.
// Los datos a cargar en la funcion son:
// void Transf_Park_Directa_IA(Vector a Transformar, cos ang referencial, sin ang referencial)

   void Transf_Park_Inversa_IA(struct VECTOR_Park *, float, float);
   
//###########################################################################################//
//						        Funcion Transf_Park_Directa_IP 						         //
//###########################################################################################//

// Definicion prototipica de la funcion " Transf_Park_Directa_IP ".

// Esta funcion permite calcular la transformada directa de Park invariante en potencia.
// Los datos a cargar en la funcion son:
// void Transf_Park_Directa_IP(Vector a Transformar, cos ang referencial, sin ang referencial)

   void Transf_Park_Directa_IP(struct VECTOR_Park *, float, float);

//###########################################################################################//
//						         Funcion Transf_Park_Inversa_IP						         //
//###########################################################################################//

// Definicion prototipica de la funcion " Transf_Park_Inversa_IP ".

// Esta funcion permite calcular la transformada inversa de Park invariante en potencia.
// Los datos a cargar en la funcion son:
// void Transf_Park_Directa_IP(Vector a Transformar, cos ang referencial, sin ang referencial)

   void Transf_Park_Inversa_IP(struct VECTOR_Park *, float, float);

// aca

//###########################################################################################//
//						      Funcion Transf_Clarke_Directa_IA 						         //
//###########################################################################################//

// Definicion prototipica de la funcion " Transf_Clarke_Directa_IA ".

// Esta funcion permite calcular la transformada directa de Clarke invariante en amplitud.
// Los datos a cargar en la funcion son:
// void Transf_Clarke_Directa_IA(Vector a Transformar)

   void Transf_Clarke_Directa_IA(struct VECTOR_Clarke *);

//###########################################################################################//
//						       Funcion Transf_Clarke_Inversa_IA						         //
//###########################################################################################//

// Definicion prototipica de la funcion " Transf_Clarke_Inversa_IA ".

// Esta funcion permite calcular la transformada inversa de Clarke invariante en amplitud.
// Los datos a cargar en la funcion son:
// void Transf_Clarke_Directa_IA(Vector a Transformar)

   void Transf_Clarke_Inversa_IA(struct VECTOR_Clarke *);
   
//###########################################################################################//
//						      Funcion Transf_Clarke_Directa_IP 						         //
//###########################################################################################//

// Definicion prototipica de la funcion " Transf_Clarke_Directa_IP ".

// Esta funcion permite calcular la transformada directa de Clarke invariante en potencia.
// Los datos a cargar en la funcion son:
// void Transf_Clarke_Directa_IP(Vector a Transformar)

   void Transf_Clarke_Directa_IP(struct VECTOR_Clarke *);

//###########################################################################################//
//						       Funcion Transf_Clarke_Inversa_IP						         //
//###########################################################################################//

// Definicion prototipica de la funcion " Transf_Clarke_Inversa_IP ".

// Esta funcion permite calcular la transformada inversa de Clarke invariante en potencia.
// Los datos a cargar en la funcion son:
// void Transf_Clarke_Directa_IP(Vector a Transformar)

   void Transf_Clarke_Inversa_IP(struct VECTOR_Clarke *);
   
#endif /*Transformadas_*/

//###########################################################################################//
//						                     FIN              	     				         //
//###########################################################################################//
