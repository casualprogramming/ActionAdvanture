// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
/**
 * 
 */


struct ACTIONADVANTURE_API CommonUtils
{
	
};

#ifdef DO_CHECK
	#ifndef condition
	
	/*If we use template metaprogramming, it is expressed as a single function, but we can't display the variable name, so we do hard-coding macros.*/

	//example: condition2_return(condition1, condition2, false);
	#define condition_return(c, ReturnValue)									{if(!ensureAlways(c))/*for profiling*/\
																						return ReturnValue;}
	#define condition2_return(c1, c2, ReturnValue)								condition_return(c1, ReturnValue); condition_return(c2, ReturnValue);
	#define condition3_return(c1, c2, c3, ReturnValue)							condition_return(c1, ReturnValue); condition2_return(c2, c3, ReturnValue);
	#define condition4_return(c1, c2, c3, c4, ReturnValue)						condition_return(c1, ReturnValue); condition3_return(c2, c3, c4, ReturnValue);

	//example: conditionf2_return(condition1, condition2, TEXT("Hello"));
	#define conditionf_return(c, ReturnValue, InFormat, ...)					{if(!ensureAlwaysMsgf(c, InFormat, ##__VA_ARGS__))/*for profiling*/\
																							return ReturnValue;}
	#define conditionf2_return(c1, c2, ReturnValue, InFormat, ...)				conditionf_return(c1, ReturnValue, InFormat, ##__VA_ARGS__); conditionf_return(c2, ReturnValue, InFormat, ##__VA_ARGS__);
	#define conditionf3_return(c1, c2, c3, ReturnValue, InFormat, ...)			conditionf_return(c1, ReturnValue, InFormat, ##__VA_ARGS__); conditionf2_return(c2, c3, ReturnValue, InFormat, ##__VA_ARGS__);
	#define conditionf4_return(c1, c2, c3, c4, ReturnValue, InFormat, ...)		conditionf_return(c1, ReturnValue, InFormat, ##__VA_ARGS__); conditionf3_return(c2, c3, c4, ReturnValue, InFormat, ##__VA_ARGS__);

	//example: condition2(condition1, condition2) //ReturnValue:void, if the conditions are not true, it will return;
	#define condition(c)														condition_return(c, );
	#define condition2(c1, c2)													{condition(c1); condition(c2);}
	#define condition3(c1, c2, c3)												{condition(c1); condition2(c2,c3);}
	#define condition4(c1, c2, c3, c4)											{condition(c1); condition3(c2,c3,c4);}
	
	#define conditionf(c, InFormat, ...)										conditionf_return(c, , InFormat, ##__VA_ARGS__);
	#define conditionf2(c1, c2, InFormat, ...)									{conditionf(c1, InFormat, ##__VA_ARGS__); conditionf(c2, InFormat, ##__VA_ARGS__);}
	#define conditionf3(c1, c2, c3, InFormat, ...)								{conditionf(c1, InFormat, ##__VA_ARGS__); conditionf2(c2,c3, InFormat, ##__VA_ARGS__);}
	#define conditionf4(c1, c2, c3, c4, InFormat, ...)							{conditionf(c1, InFormat, ##__VA_ARGS__); conditionf3(c2,c3,c4, InFormat, ##__VA_ARGS__);}

	//example: conditionb2(condition1, condition2) //ReturnValue:false, if the conditions are not true, it will return false;
	#define conditionb(c)														condition_return(c,false);
	#define conditionb2(c1, c2)													{conditionb(c1); conditionb(c2);}
	#define conditionb3(c1, c2, c3)												{conditionb(c1); conditionb2(c2,c3);}
	#define conditionb4(c1, c2, c3, c4)											{conditionb(c1); conditionb3(c2,c3,c4);}
	
	#define conditionbf(c, InFormat, ...)										conditionf_return(c,false, InFormat, ##__VA_ARGS__);
	#define conditionbf2(c1, c2, InFormat, ...)									{conditionbf(c1, InFormat, ##__VA_ARGS__); conditionbf(c2, InFormat, ##__VA_ARGS__);}
	#define conditionbf3(c1, c2, c3, InFormat, ...)								{conditionbf(c1, InFormat, ##__VA_ARGS__); conditionbf2(c2,c3, InFormat, ##__VA_ARGS__);}
	#define conditionbf4(c1, c2, c3, c4, InFormat, ...)							{conditionbf(c1, InFormat, ##__VA_ARGS__); conditionbf3(c2,c3,c4, InFormat, ##__VA_ARGS__);}
	#endif
#endif // 
