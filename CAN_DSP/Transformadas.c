//########################################################################################################//
//                                                                                                        //        
//              Esta libreria realiza el calculo de las transformadas de Park y Clarke                    //
//              directa e inversa invariantes en amplitud y potencia.                                     //
//              Los ejes del referencial son considerados como: eje d es el eje directo                   //                           
//              y el eje q es el eje en cuadratura que esta atrasado 90º respecto al eje                  //
//              d. El eje d es el que se pega al vector Va.                                               //                                         
//                                                                                                        //
//              Nombre: Transformadas.c                                                                   //
//                                                                                                        //
//              Federico M. Serra                                                                         //
//                                                                                                        //
//              17/11/2010                                                                                //
//                                                                                                        //
//########################################################################################################//

#include "Transformadas.h"

//########################################################################################################//
//						               Funcion Transf_Park_Directa_IA         						      //
//########################################################################################################//

// Esta funcion permite calcular la transformada directa de Park invariante en amplitud.

void Transf_Park_Directa_IA(struct VECTOR_Park *H,float COS_T, float SIN_T)
{

H->Xd = (2.0/3.0)*(H->Xa*COS_T + H->Xb*(-0.5*COS_T + 0.866025403*SIN_T) + H->Xc*(-0.5*COS_T - 0.866025403*SIN_T));  
H->Xq = (2.0/3.0)*(H->Xa*SIN_T + H->Xb*(-0.5*SIN_T - 0.866025403*COS_T) + H->Xc*(-0.5*SIN_T + 0.866025403*COS_T));  
H->X0 = (1.0/3.0)*(H->Xa + H->Xb + H->Xc);  
        
}

//########################################################################################################//
//						               Funcion Transf_Park_Inversa_IA         						      //
//########################################################################################################//

// Esta funcion permite calcular la transformada inversa de Park invariante en amplitud.

void Transf_Park_Inversa_IA(struct VECTOR_Park *H,float COS_T, float SIN_T)
{

H->Xa = H->Xd*COS_T                            + H->Xq*SIN_T                            + H->X0;
H->Xb = H->Xd*(-0.5*COS_T + 0.866025403*SIN_T) + H->Xq*(-0.5*SIN_T - 0.866025403*COS_T) + H->X0;
H->Xc = H->Xd*(-0.5*COS_T - 0.866025403*SIN_T) + H->Xq*(-0.5*SIN_T + 0.866025403*COS_T) + H->X0;	
          
}

//########################################################################################################//
//						               Funcion Transf_Park_Directa_IP         						      //
//########################################################################################################//

// Esta funcion permite calcular la transformada directa de Park invariante en potencia.

void Transf_Park_Directa_IP(struct VECTOR_Park *H,float COS_T, float SIN_T)
{

H->Xd = (0.81649658)*(H->Xa*COS_T + H->Xb*(-0.5*COS_T + 0.866025403*SIN_T) + H->Xc*(-0.5*COS_T - 0.866025403*SIN_T));  
H->Xq = (0.81649658)*(H->Xa*SIN_T + H->Xb*(-0.5*SIN_T - 0.866025403*COS_T) + H->Xc*(-0.5*SIN_T + 0.866025403*COS_T));  
H->X0 = (0.81649658)*(H->Xa*0.707106781 + H->Xb*0.707106781 + H->Xc*0.707106781);  
        
}

//########################################################################################################//
//						               Funcion Transf_Park_Inversa_IP         						      //
//########################################################################################################//

// Esta funcion permite calcular la transformada inversa de Park invariante en potencia.

void Transf_Park_Inversa_IP(struct VECTOR_Park *H,float COS_T, float SIN_T)
{

H->Xa = (0.81649658)*(H->Xd*COS_T                            + H->Xq*SIN_T                            + H->X0*0.707106781);
H->Xb = (0.81649658)*(H->Xd*(-0.5*COS_T + 0.866025403*SIN_T) + H->Xq*(-0.5*SIN_T - 0.866025403*COS_T) + H->X0*0.707106781);
H->Xc = (0.81649658)*(H->Xd*(-0.5*COS_T - 0.866025403*SIN_T) + H->Xq*(-0.5*SIN_T + 0.866025403*COS_T) + H->X0*0.707106781);	
          
}

//########################################################################################################//
//						              Funcion Transf_Clarke_Directa_IA         						      //
//########################################################################################################//

// Esta funcion permite calcular la transformada directa de Clarke invariante en amplitud.

void Transf_Clarke_Directa_IA(struct VECTOR_Clarke *H)
{

H->X0      = (2.0/3.0)*(H->Xa*0.707106781 + H->Xb*0.707106781 + H->Xc*0.707106781);  
H->Xalpha  = (2.0/3.0)*(H->Xa*1           + H->Xb*(-0.5)      + H->Xc*(-0.5));  
H->Xbeta   = (2.0/3.0)*(H->Xa*0           + H->Xb*0.866025403 + H->Xc*(-0.866025403));  
        
}

//########################################################################################################//
//						              Funcion Transf_Clarke_Inversa_IA         						      //
//########################################################################################################//

// Esta funcion permite calcular la transformada inversa de Clarke invariante en amplitud.

void Transf_Clarke_Inversa_IA(struct VECTOR_Clarke *H)
{

H->Xa = H->X0*0.707106781 + H->Xalpha*1      + H->Xbeta*0;
H->Xb = H->X0*0.707106781 + H->Xalpha*(-0.5) + H->Xbeta*0.866025403;
H->Xc = H->X0*0.707106781 + H->Xalpha*(-0.5) + H->Xbeta*(-0.866025403);	
          
}

//########################################################################################################//
//						              Funcion Transf_Clarke_Directa_IP         						      //
//########################################################################################################//

// Esta funcion permite calcular la transformada directa de Clarke invariante en potencia.

void Transf_Clarke_Directa_IP(struct VECTOR_Clarke *H)
{

H->X0      = (0.81649658)*(H->Xa*0.707106781 + H->Xb*0.707106781 + H->Xc*0.707106781);
H->Xalpha  = (0.81649658)*(H->Xa*1           + H->Xb*(-0.5)      + H->Xc*(-0.5));  
H->Xbeta   = (0.81649658)*(H->Xa*0           + H->Xb*0.866025403 + H->Xc*(-0.866025403));  
        
}

//########################################################################################################//
//						              Funcion Transf_Clarke_Inversa_IP         						      //
//########################################################################################################//

// Esta funcion permite calcular la transformada inversa de Clarke invariante en potencia.

void Transf_Clarke_Inversa_IP(struct VECTOR_Clarke *H)
{

H->Xa = (0.81649658)*(H->X0*0.707106781 + H->Xalpha*1      + H->Xbeta*0);
H->Xb = (0.81649658)*(H->X0*0.707106781 + H->Xalpha*(-0.5) + H->Xbeta*0.866025403);
H->Xc = (0.81649658)*(H->X0*0.707106781 + H->Xalpha*(-0.5) + H->Xbeta*(-0.866025403));
          
}

//########################################################################################################//
//						                            FIN                     						      //
//########################################################################################################//
