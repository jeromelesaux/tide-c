#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define pi 3.141592653589793
/* declaration des variables globales */


#ifdef CPC
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif


#define HARMOPATH "./Data/harmo.har"

/***************************************************************** 
* This program is free software; you can redistribute it and/or  *
* modify it under the terms of the GNU General Public License    * 
* as published by the Free Software Foundation; either version 2 *
* of the License, or (at your option) any later version.         * 
* for any informations mail me : jlesaux@ifrance.com             *
******************************************************************/

// double vo[21]; /* contenant les valeurs des vecteurs ondes */
double s,h,p,n,d,p1,xj,yj,vij;
/* T = temps en heure decimale, t = temps en jour decimale depuis 1980 */

char* port, *descPort;

/*************************************************************************/
/* declaration des resultats des initialisations pour chaque type d'onde */
/* avec R.0 = R.0 pour chaque type d'onde, dR. = R.(24) - R.(0)          */
/* G.0 = Q.0, d.Q d.Q = Q(24) - Q(0)                                     */
/*************************************************************************/
typedef struct ConstanteHarmonic {
		double amplitude;
		double situation;
} ConstanteHarmonic;

typedef struct VecteurOnde {
	double z0;
	ConstanteHarmonic Sa;
	ConstanteHarmonic Q1;
	ConstanteHarmonic O1;
	ConstanteHarmonic K1;
	ConstanteHarmonic N2;
	ConstanteHarmonic M2;
	ConstanteHarmonic S2;
	ConstanteHarmonic MN4;
	ConstanteHarmonic M4;
	ConstanteHarmonic MS4;
	
} VecteurOnde;


char pmm[15],pms[15],bmm[15],bms[15];
double hbms,hpms,hbmm,hpmm,cfm,cfs,coeffZ0,prefZ0;
double temps;
double jourMaree, moisMaree, anneeMaree;
double a[5][11],g[5][11],coeffA[5][11],coeffG[5][11],prefA[5][11],prefG[5][11],phi0[5],phi24[5],r0[5],r24[5];
VecteurOnde VO;

int getDateElement(char *date,int element) {
	int i=0,h=0,j=1,find;
	char buf[11];
	while (i < strlen(date) && j <= element) {
		if (date[i] != '/') {
			if (element == j) {
				buf[h] = date[i] ;	
				h++;
			}
		}
		else 
			j++; 
		i++;
		
	}
	buf[h+1] = '\0';
	find = atoi(buf);
	return find; 
}

char* getStringElement(char * string,int element) {
	int i=0,h=0,j=1;
	char* buf ;
	
	if ( (buf = (char*) malloc (252*sizeof(char))) == NULL) {
		fprintf(stderr,"erreur d'allocation memoire de la variable buf\n");
		exit( EXIT_FAILURE );
	}
	while (i < strlen(string) && j <= element) 	{
		if (string[i] != '|') {
			if (element == j) {
				buf[h] = string[i];
				h++;
			}
		}
		else 
			j++; 
		i++;
	}
	buf[h] = '\0';
	return buf;
}

char* getPort (char* string) {
	char* portString;
	portString = getStringElement(string,1);
	return portString ;
}

char* getPortDescription (char *string) {
	char* portDescString;
	portDescString = getStringElement(string,2);
	return portDescString;
}

void getVo (char* string) {
	int i= 3;
	
	VO.z0 = (double) atof(getStringElement(string,i++));
	VO.Sa.amplitude = (double) atof(getStringElement(string,i++));
	VO.Sa.situation  = (double) atof(getStringElement(string,i++));
	VO.Q1.amplitude = (double) atof(getStringElement(string,i++));
	VO.Q1.situation = (double) atof(getStringElement(string,i++));
	VO.O1.amplitude = (double) atof(getStringElement(string,i++));
	VO.O1.situation = (double) atof(getStringElement(string,i++));
	VO.K1.amplitude = (double) atof(getStringElement(string,i++));
	VO.K1.situation = (double) atof(getStringElement(string,i++));
	VO.N2.amplitude = (double) atof(getStringElement(string,i++));
	VO.N2.situation = (double) atof(getStringElement(string,i++));
	VO.M2.amplitude = (double) atof(getStringElement(string,i++));
	VO.M2.situation = (double) atof(getStringElement(string,i++));
	VO.S2.amplitude = (double) atof(getStringElement(string,i++));
	VO.S2.situation = (double) atof(getStringElement(string,i++));
	VO.MN4.amplitude = (double) atof(getStringElement(string,i++));
	VO.MN4.situation = (double) atof(getStringElement(string,i++));
	VO.M4.amplitude = (double) atof(getStringElement(string,i++));
	VO.M4.situation = (double) atof(getStringElement(string,i++));
	VO.MS4.amplitude = (double) atof(getStringElement(string,i++));
	VO.MS4.situation = (double) atof(getStringElement(string,i++));
}

void printVo() {
	printf("HARMONICS CONSTANTS\n");
	printf("Z0:%5.3f\n",VO.z0);
	printf("Sa.A:%5.3f\nSa.G:%5.3f\n",VO.Sa.amplitude,VO.Sa.situation);
	printf("Q1.A:%5.3f\nQ1.G:%5.3f\n",VO.Q1.amplitude,VO.Q1.situation);
	printf("O1.A:%5.3f\nO1.G:%5.3f\n",VO.O1.amplitude,VO.O1.situation);
	printf("K1.A:%5.3f\nK1.G:%5.3f\n",VO.K1.amplitude,VO.K1.situation);
	printf("N2.A:%5.3f\nN2.G:%5.3f\n",VO.N2.amplitude,VO.N2.situation);
	printf("M2.A:%5.3f\nM2.G:%5.3f\n",VO.M2.amplitude,VO.M2.situation);
	printf("S2.A:%5.3f\nS2.G:%5.3f\n",VO.S2.amplitude,VO.S2.situation);
	printf("MN4.A:%5.3f\nMN4.G:%5.3f\n",VO.MN4.amplitude,VO.MN4.situation);
	printf("M4.A:%5.3f\nM4.G:%5.3f\n",VO.M4.amplitude,VO.M4.situation);
	printf("MS4.A:%5.3f\nMS4.G:%5.3f\n",VO.MS4.amplitude,VO.MS4.situation);

}




int sng(double v) {
	if ( v < 0 ) return -1;
	else return 1;
}


void convDateHeure(char * date, double v) {
	double heure = floor(v);
	double minute = floor(floor((v - floor(v))*100.0)*0.6);
	
	sprintf(date,"%2.0fH%2.0f %2.0f/%2.0f/%2.0f",heure,minute,jourMaree,moisMaree,anneeMaree);
}
 
double convHauteur(double v) {
 	//printf("v:%f\n",v);
 	return floor(v/10.)/100;
}
 
void initPort(int isCoef) {
 	int j,i;
 	// initialisation des tableaux
 	for ( j = 0 ; j < 5 ; j++ ) {
 		for ( i =0; i<11 ; i++ ) {
 			a[j][i]=0;
 			g[j][i]=0;
 		}
 	}
 	if (isCoef) {
 		a[0][0] = 0;
 		a[1][0] = 0;
 		a[1][1] = 0;
 		a[1][2] = 0;
 		a[1][3] = 0;
 		a[1][4] = 0;
 		a[1][5] = 0;
 		a[2][0] = 668.0;
 		a[2][1] = 136.0;
 		a[2][2] = 245.0;
 		a[2][3] = (1.0/7.6)*136.0;
 		a[2][4] = (1.0/6.3)*136.0;
 		a[2][5] = (1.0/5.6)*136.0;
 		a[2][6] = (-1.0/35.0)*668.0;
 		a[2][7] = (1.0/3.7)*245.0;
 		a[2][8] = (1.0/17.0)*245.0;
 		a[2][9] = (-1.0/27.0)*668.0;
 		a[2][10] = (1.0/12.0)*245.0;
 		a[4][0] = 0;
 		a[4][1] = 0;
 		a[4][2] = 0;
 		
 		g[0][0] = 0;
 		g[1][0] = 0;
 		g[1][1] = 0;
 		g[1][2] = 0;
 		g[1][3] = 0;
 		g[1][4] = 0;
 		g[1][5] = 0;
 		g[2][0] = 138.0;
 		g[2][1] = 119.0;
 		g[2][2] = 178.0;
 		g[2][3] = 119.0;
 		g[2][4] = 119.0;
 		g[2][5] = 119.0;
 		g[2][6] = 138.0;
 		g[2][7] = 178.0;
 		g[2][8] = -105.0;
 		g[2][9] = 138.0;
 		g[2][10] = 178.0;
 		g[4][0] = 0;
 		g[4][1] = 0;
 		g[4][2] = 0;
 		coeffZ0=0;
 		for ( j=0;j<5;j++) {
 			for ( i=0;i<11;i++) {
 				coeffA[j][i] = a[j][i];
 				coeffG[j][i] = g[j][i];
 			}
 		}
 	}
 	else {
 		a[0][0] = VO.Sa.amplitude;
 		a[1][0] = VO.K1.amplitude;
 		a[1][1] = VO.O1.amplitude;
 		a[1][2] = VO.Q1.amplitude;
 		a[1][3] = (-1.0/3.0) * VO.K1.amplitude;
 		a[1][4] = (1.0/5.3) * VO.O1.amplitude;
 		a[1][5] = (1.0/7.4) * VO.K1.amplitude;
 		a[2][0] = VO.M2.amplitude;
 		a[2][1] = VO.N2.amplitude;
 		a[2][2] = VO.S2.amplitude;
 		a[2][3] = (1.0/7.6) * VO.N2.amplitude;
 		a[2][4] = (1.0/6.3) * VO.N2.amplitude;
 		a[2][5] = (1.0/5.3) * VO.N2.amplitude;
 		a[2][6] = (-1.0/35.0) * VO.M2.amplitude;
 		a[2][7] = (1.0/3.7) * VO.S2.amplitude;
 		a[2][8] = (1.0/17.0) * VO.S2.amplitude;
 		a[2][9] = (-1.0/27.0) * VO.M2.amplitude;
 		a[2][10] = (1.0/12.0) * VO.S2.amplitude;
 		a[4][0] = VO.MN4.amplitude;
 		a[4][1] = VO.M4.amplitude;
 		a[4][2] = VO.MS4.amplitude;
 		
 		g[0][0] = VO.Sa.situation;
 		g[1][0] = VO.K1.situation;
 		g[1][1] = VO.O1.situation;
 		g[1][2] = VO.Q1.situation;
 		g[1][3] = VO.K1.situation;
 		g[1][4] = VO.O1.situation;
 		g[1][5] = VO.K1.situation;
 		g[2][0] = VO.M2.situation;
 		g[2][1] = VO.N2.situation;
 		g[2][2] = VO.S2.situation;
 		g[2][3] = VO.N2.situation;
 		g[2][4] = VO.N2.situation;
 		g[2][5] = VO.N2.situation;
 		g[2][6] = VO.M2.situation;
 		g[2][7] = VO.S2.situation;
 		g[2][8] = VO.S2.situation - 283.0;
 		g[2][9] = VO.M2.situation;
 		g[2][10] = VO.S2.situation;
 		g[4][0] = VO.MN4.situation;
 		g[4][1] = VO.M4.situation;
 		g[4][2] = VO.MS4.situation;
 		
 		prefZ0=VO.z0;
 		for ( j=0;j<5;j++) {
 			for ( i=0;i<11;i++) {
// 				printf("a[%d][%d]:%f,g[%d][%d]:%f\n",j,i,a[j][i],j,i,g[j][i]);
 				prefA[j][i] = a[j][i];
 				prefG[j][i] = g[j][i];
 			}
 		}
 	}
}
 
void initMaree(int isCoef) {
 	int i,j;
 	double t,t2,test;
 	double n1[5][11] = { {0,0,0,0,0,0,0,0,0,0,0}, {0,-2,-3,0,-2,0,0,0,0,0,0}, {-2,-3,0,-4,-4,-3,-1,0,0,-2,0}, {0,0,0,0,0,0,0,0,0,0,0}, {-5,-4,-2,0,0,0,0,0,0,0,0}};
 	double n2[5][11] = {{-1,0,0,0,0,0,0,0,0,0,0},{1,1,1,-1,1,1,0,0,0,0,0},{2,2,0,2,4,4,2,2,-1,2,2},{0,0,0,0,0,0,0,0,0,0,0},{4,4,2,0,0,0,0,0,0,0,0}};
 	double n3[5][11] = {{0,0,0,0,0,0,0,0,0,0,0},{0,0,1,0,0,0,0,0,0,0,0},{0,1,0,2,0,-1,-1,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0,0,0,0}};
 	double n4[5][11] = {{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,-1,-1,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,-1,1},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}};
 	double n5[5][11] = {{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}};
 	double n6[5][11] = {{0,0,0,0,0,0,0,0,0,0,0},{1,-1,-1,1,-1,1,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}};
 	if ( isCoef ) {
 		VO.z0 = coeffZ0 ;
 		for ( j=0;j<5;j++) {
 			for (i=0;i<11;i++) {
 				a[j][i] = coeffA[j][i];
 				g[j][i] = coeffG[j][i];
 			}
 		}
 	}
 	else {
 		VO.z0=prefZ0;
 		for ( j=0;j<5;j++) {
 			for (i=0;i<11;i++) {
 				a[j][i] = prefA[j][i];
 				g[j][i] = prefG[j][i];
 				//printf("a[%d][%d]:%f,g[%d][%d]:%f\n",j,i,a[j][i],j,i,g[j][i]);
 			}
 		}
 	}
 	t=0;
 	t2 = floor(30.6001*(1.0+moisMaree +12.0 * floor(1.0/(moisMaree+1.0)+0.7)))+floor(365.25*(anneeMaree - floor(1.0/(moisMaree+1.0)+0.7))) + jourMaree + t/24.0 - 723258.0;
 	h = 279.82 + 0.98564734 *t2;
 	s = 78.16+13.17639673 * t2;
 	p = 349+0.11140408*t2;
 	n = 208.1 + 0.05295392 * t2;
 	p1 = 282.6 +0.000047069 * t2;
 	d=90.0;
 	//printf("t:%f,t2:%f,h:%f,s:%f,p:%f,n:%f,p1:%f,d:%f\n",t,t2,h,s,p,n,p1,d);
 	for ( j=0;j<5;j++) {
 		xj=0;
 		yj=0;
 		for (i=0;i<11;i++) {
 			//printf("j+1:%d,j:%d\n",(j+1),j);
 			vij = 15 * (j) * t + n1[j][i]* s + n2[j][i]*h + n3[j][i]*p + n4[j][i] *n+n5[j][i]*p1+n6[j][i]*d;
 			xj = xj + a[j][i] * cos(pi/180*(vij-g[j][i]));
 			yj = yj + a[j][i] * sin(pi/180*(vij-g[j][i]));
 			//printf("vij:%f,xj:%f,yj:%f,a[%d][%d]:%f,g[%d][%d]:%f\n",vij,xj,yj,j,i,a[j][i],j,i,g[j][i]);
 		}
 		r0[j] = sqrt((xj*xj)+(yj*yj));
 		//printf("r0[%d]:%f\n",j,r0[j]);
 		if ( r0[j] == 0 )	{
 			phi0[j] = 90.0;
 		}
 		else {
 			test = 180.0/pi*(asin(yj/r0[j]));
 			if ( test < 0 ) {
 				phi0[j] = 180./pi*(acos(xj/r0[j]))*-1;
 			}
 			else {
 				phi0[j] = 180./pi*(acos(xj/r0[j]));
 			}
 		}
 		//printf("ph0[%d]:%f\n",j,phi0[j]);
 	}
 	t = 24. ;
 	t2 =  floor(30.6001*(1.0+moisMaree +12.0 * floor(1.0/(moisMaree+1.0)+0.7)))+floor(365.25*(anneeMaree - floor(1.0/(moisMaree+1.0)+0.7))) + jourMaree + t/24.0 - 723258.0;
 	h = 279.82 + 0.98564734 *t2;
 	s = 78.16+13.17639673 * t2;
 	p = 349+0.11140408*t2;
 	n = 208.1 + 0.05295392 * t2;
 	p1 = 282.6 +0.000047069 * t2;
 	d=90;
 	//printf("t:%f,t2:%f,h:%f,s:%f,p:%f,n:%f,p1:%f,d:%f\n",t,t2,h,s,p,n,p1,d);
 	for ( j=0;j<5;j++) {
 		xj=0;
 		yj=0;
 		for (i=0;i<11;i++) {
 			vij = 15 * (j) * t + n1[j][i]* s + n2[j][i] *h + n3[j][i ]*p + n4[j][i] *n + n5[j][i]*p1+ n6[j][i]*d;
 			xj = xj + a[j][i] * cos(pi/180*(vij-g[j][i]));
 			yj = yj + a[j][i] * sin(pi/180*(vij-g[j][i]));
 			//printf("vij:%f,xj:%f,yj:%f,a[%d][%d]:%f,g[%d][%d]:%f\n",vij,xj,yj,j,i,a[j][i],j,i,g[j][i]);
 		}
 		r24[j] = sqrt((xj*xj)+(yj*yj));
 		//printf("r24[%d]:%f\n",j,r24[j]);
 		if (r24[j]==0) {
 			phi24[j] = 90;
 		}
 		else  {
 			test = 180/pi*(asin(yj/r24[j]));
 			if ( test < 0 ) {
 				phi24[j] = 180/pi*(acos(xj/r24[j]))*-1;
 			}
 			else 	{
 				phi24[j] = 180/pi*(acos(xj/r24[j]));
 			}
 		}
 		//printf("ph24[%d]:%f\n",j,phi24[j]);
 	}
 }
 
double calcHauteur(double val) {
 	int j;
 	double rj,deltaj,phij,hauteur;
 	
 	hauteur = VO.z0 *10.0;
 	//printf("hauteur:%f,val:%f\n",hauteur,val);
 	for ( j=0;j<5;j++) {
 		rj = r0[j] + val /24.0 * ( r24[j] - r0[j] );
 		deltaj = phi24[j] - phi0[j];
 		if ( deltaj < -180. ) {
 			deltaj += 360.;
 		}
 		if ( deltaj > 180. ) {
 			deltaj -= 360.;
 		}
 		phij = phi0[j] + val / 24. * (j*360.0 +deltaj );
 		//printf("deltaj:%f,phij:%f\n",deltaj,phij);
 		hauteur = hauteur + rj * cos(phij * pi /180.0);
 	}
 	//printf("hauteur:%f\n",hauteur);
 	if ( hauteur < 0.0 ) hauteur = abs(hauteur) ;
 	return hauteur;
}
 
void calculMaree(int isCoef) {
 	double sigma,sigma2,dt,pif,deltapm,t,rj,deltaj,phij;
 	double tpbs[4];
 	int j,pb;
 	sigma = sigma2 = dt = pb = pif = deltapm = 0;
 	initMaree(isCoef);
 	j=2;
 	if ( phi0[j] < 0) deltapm = 0;
 	else deltapm = 1;
 	t=0;
 	for ( j=0;j<5;j++) {
 		rj = r0[j] + t/24.0 * (r24[j] - r0[j]);
 		//printf("rj:%f,r0[%d]:%f,r24[%d]:%f\n",rj,j,r0[j],j,r24[j]);
 		deltaj= phi24[j] - phi0[j];
 		if ( deltaj < -180. ) deltaj += 360.;
 		if ( deltaj > 180. ) deltaj -= 360.;
 		phij = phi0[j] + t / 24. * (j*360.+deltaj);
 		sigma = sigma + pi / 180. * rj * (15. * j +deltaj / 24.) *sin(phij * pi /180.) - (r24[j] - r0[j]) / 24. * cos(phij *pi /180.);
 		//printf("j:%d,sigma:%f\n",j,sigma);
 	}
 	for ( pb =0;pb<=3;pb++) {
 		dt=0.4;
 		for ( pif=0; pif<=1; pif++) {
 			do {
 				t = t+dt;
 				sigma2=sigma;
 				sigma=0;
 				for ( j=0;j<5;j++) {
 					rj = r0[j] + t/24. * (r24[j] - r0[j]);
 					deltaj= phi24[j] - phi0[j];
 					if ( deltaj < -180. ) deltaj += 360.;
 					if ( deltaj > 180. ) deltaj -= 360.;
 					phij = phi0[j] + t / 24. * (j*360.+deltaj);
 					sigma = sigma + pi / 180 * rj * (15. * j +deltaj / 24.) *sin(phij * pi /180.) - (r24[j] - r0[j]) / 24. * cos(phij *pi /180.);
 				}
 				//printf("sigma:%f,sigma2:%f,t:%f\n",sigma,sigma2,t);
 			}while (sng(sigma) == sng(sigma2));
 			
 			t = t -dt;
 			if ( pif == 0) 	{
 				sigma =sigma2;
 				dt=0.01;
 			}
 	 }
 		tpbs[pb]=t;
 		//printf("tpbs[%d]=%f\n",pb,t);
 }
 	if ( isCoef ) 	{
 		cfm = (calcHauteur(tpbs[0])+deltapm)/10;
 		cfs = (calcHauteur(tpbs[3])+deltapm)/10;
 		//printf("cfm:%2.2f,cfs:%2.2f\n",cfm,cfs);
 	}
 	else 	{
 		//printf("deltapm:%f\n",deltapm);
 		convDateHeure(bmm,tpbs[0]);
 		convDateHeure(pmm,tpbs[1]);
 		convDateHeure(bms,tpbs[2]);
 		convDateHeure(pms,tpbs[3]);
 		hbmm = convHauteur(calcHauteur(tpbs[0]+deltapm));
 		hpmm = convHauteur(calcHauteur(tpbs[1]+deltapm));
 		hbms = convHauteur(calcHauteur(tpbs[2]-deltapm));
 		hpms = convHauteur(calcHauteur(tpbs[3]-deltapm));
 		//printf("hbms:%f\n",hbms);
 	}
 }
 
 void choixPort(char* string)  {
 	FILE* Stream;
 	int cont = 1;
 	int i=0;
 	char buffer[1024];
 	char user[5];
 	int choix=-1;
 	if ( ( Stream = fopen( HARMOPATH,"r" ) ) ==NULL ) {
 		fprintf(stderr,"No Data file...Exiting\n");
 		exit( EXIT_FAILURE );
 	}
 	while ( cont ) 	{
 		fgets( buffer, 1024, Stream );
 		printf("Choix : %.3d | Port : %s | Description : %s \n",i, getStringElement(buffer,1), getStringElement(buffer,2));
 		i++; 
 		if ( ( i % 20 ) == 0) { // on s'arrete pour demander l'avis 	
 			printf("Votre choix  (entrer le numero de choix, [enter] pour continuer ) :");
 			fgets(user,1024,stdin);
 			//printf("user:%s\n",user);
 			if ( strlen(user) > 1 && sscanf(user,"%d",&choix)==1);
 				//printf ("user:|%s|%d|choix:%d\n",user,strlen(user),choix);
 			else choix=-1;
 			
 			system("clear");
 			
 		}
 		if ( feof( Stream ) ) { // au cas ou l'utilisateur a deja tout parcouru 
 			i=0;
 			rewind( Stream );
 		}
 		if ( choix != -1 ) {
 			cont =0;
 		}
 	}
 	
 	i =-1;
 	rewind( Stream );
 	while ( i!=choix && !feof( Stream ) ) {
 		fgets( buffer,  1024, Stream );
 		i++;
 	}
 	if (feof( Stream ) )  {
 		fprintf(stderr,"Your choice exceed the possibilities...Exiting\n");
 		exit( EXIT_FAILURE );
 	}
 	strcpy(string,buffer);
 	fclose( Stream );
 }
 
 int main (int argc, char** argv) {

	char string[1024];
	time_t dateOfDay;
	struct tm *timeStruct; 
	int jm,mm,am;

	if (argc != 1 && argc != 2 ) {
		printf("%s :\n\tUSAGE: %s <date> (like 12/01/1999) \n\n",argv[0],argv[0]);
		exit( EXIT_FAILURE );
	}
	

	if ( 2 == argc ) {
		dateOfDay = time(NULL);
		timeStruct = localtime( &dateOfDay) ;
		printf("Date of the day : %s",asctime( localtime( &dateOfDay ) ) );
		//printf("Day : %d, Year : %d, Month : %d\n",timeStruct->tm_mday, timeStruct->tm_year + 1900, timeStruct->tm_mon+1 ); 
		jourMaree = timeStruct->tm_mday ;
		moisMaree = timeStruct->tm_mon +1 ; // month 0 to 11 
		anneeMaree = timeStruct->tm_year + 1900 ; // tm_year give the number of years till 1900

	}
	else {
		//printf("Entre dans le else des arguments %s (%d)\n",argv[2],sscanf(argv[2],"%f/%f/%f",&jourMaree,&moisMaree,&anneeMaree));
#ifndef CPC
		
		if ( 3 != sscanf(argv[1],"%d/%d/%d",&jm,&mm,&am ) )  {
			fprintf(stderr,"Can't read the date (%s).\n",argv[2]);
			exit( EXIT_FAILURE);
		}
		jourMaree = (double) jm;
		moisMaree = (double) mm;
		anneeMaree = (double) am;

#endif
#ifdef CPC
		
		jourMaree = getDateElement(argv[1],1);
		moisMaree = getDateElement(argv[1],2);
		anneeMaree = getDateElement(argv[1],3);
		
#endif 
	}
	printf ("----------------------------------------------------\n");
	printf (" DAY:%.0f, MONTH:%2.0f, YEAR:%4.0f\n",jourMaree,moisMaree,anneeMaree);
	choixPort(string);
	
	port = getPort (string) ;
	descPort = getPortDescription (string) ;

	printf(" PORT:%.10s, DESCRIPTION:%.28s\n",port,descPort);
	printf("----------------------------------------------------\n");
	printf("DATE  : %2.0f/%2.0f/%2.0f \n",jourMaree,moisMaree,anneeMaree);

	
	
	getVo(string);
	
	//printVo();
	initPort(0);
	//printf("Sortie de initPort(0)\n");
	calculMaree(0);
	//printf("Sortie de calculMaree(0)\n");
	initPort(1);
	//printf("Sortie de initPort(1)\n");
	calculMaree(1);
	//printf("Sortie de calculMaree(1)\n");
	
	printf("                      Matin          |               Soir            \n");
	printf("Plein Mer : %s (%.2f)  |  %s (%.2f) \n",pmm,hpmm,pms,hpms);
	printf("Basse Mer : %s (%.2f)  |  %s (%.2f) \n",bmm,hbmm,bms,hbms);
	printf("Coefficient :           %2.2f        |               %2.2f               \n",cfm,cfs);
	
 	return EXIT_SUCCESS;
 	
 }
 
