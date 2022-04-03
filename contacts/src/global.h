//
// Created by blindinlight on 4/3/22.
//
#ifndef CONTACT_GLOBAL_H
#define CONTACT_GLOBAL_H

#ifdef CONTACT_CONTACT_H
#define EXTERN
#else
#define EXTERN extern
#endif
#ifdef CONTACT_CONTACT_CPP
EXTERN unsigned int name_wid;
EXTERN unsigned int adress_wid;
EXTERN unsigned int tele_wid;
EXTERN unsigned int sex_wid;
EXTERN unsigned int age_wid;
EXTERN unsigned int total_wid;
#endif
#endif