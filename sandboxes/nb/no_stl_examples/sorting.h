/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sorting.h
 * Author: someone
 *
 * Created on July 6, 2016, 7:52 AM
 */

#ifndef SORTING_H
#define SORTING_H

void bubble_sort(int* a, int n)
{
  for (int i=n-1; i>=0; i--)
  {
    for (int j=0; j<i; j++)
    {
      if (a[j] > a[j+1])
      {
        int tmp = a[j];
        a[j] = a[j+1];
        a[j+1] = tmp;
      }
    }
  }
}

void quick_sort(int* a, int l, int r)
{
    int i = l, j = r, m = a[(l + r) / 2];
    do {
        while (a[i] < m) i++;
        while (a[j] > m) j--;
 
        if(i <= j) {
            if (i < j) swap(a[i], a[j]);
            i++;
            j--;
        }
    } while (i <= j);
    if (i < r)
        quick_sort(a, i, r);
    if (l < j)
        quick_sort(a, l,j);
}

#endif /* SORTING_H */

