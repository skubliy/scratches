/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   card_shifter.h
 * Author: someone
 *
 * Created on June 29, 2016, 6:34 PM
 */

#ifndef CARD_SHIFTER_H
#define CARD_SHIFTER_H

void card_shifter(int* arr, size_t sz){
    std::srand(time(0));
    for(int i=0;i<sz;i++){
        int j = std::rand()%sz;
        int x= arr[i];
        arr[i]=arr[j];
        arr[j]=x;
    }
    
}


#endif /* CARD_SHIFTER_H */

