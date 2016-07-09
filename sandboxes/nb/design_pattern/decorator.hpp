/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   decorator.hpp
 * Author: someone
 *
 * Created on June 20, 2016, 10:59 AM
 */

#ifndef DECORATOR_HPP
#define DECORATOR_HPP

class AComponent {
public:
	virtual void operation() = 0;
        virtual ~AComponent(){}
};
 
class Component : public AComponent {
public:
	virtual void operation() {
		std::cout<<"World!"<<std::endl;
	}
};
 
class DecoratorOne : public AComponent {
	std::shared_ptr<AComponent> m_component;
 
public:
	DecoratorOne(AComponent* component): m_component(component) {}
 
	virtual void operation() {
		std::cout << ", ";
		m_component->operation();
	}
};
 
class DecoratorTwo : public AComponent {
	std::shared_ptr<AComponent> m_component;
 
public:
	DecoratorTwo(AComponent* component): m_component(component) {}
 
	virtual void operation() {
		std::cout << "Hello";
		m_component->operation();
	}
};

#endif /* DECORATOR_HPP */

