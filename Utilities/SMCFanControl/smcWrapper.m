/*
 *	FanControl
 *
 *	Copyright (c) 2006 Hendrik Holtmann
*
 *	smcWrapper.m - MacBook(Pro) FanControl application
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#import "smcWrapper.h"

//NSString * const smc_checksum=@"75a31ec0a358ae4efcb9835660b453a44eef6e3";

@implementation smcWrapper
	io_connect_t conn;

+(void)init{
	SMCOpen(&conn);
}

+(float)get_voltage:(UInt32Char_t)key {
	SMCVal_t val;
	SMCReadKey2(key, &val, conn);
	//printf("0x%x%x\n",val.bytes[0]&0xff,val.bytes[1]&0xff);
	UInt16 fp2e=(val.bytes[0]<<8)|(val.bytes[1]&0xff);
	//printf("0x%x\n",fp2e);
	UInt16 dec=fp2e>>14;
	UInt16 frc=(fp2e&0x3ff0)>>4;
	return (float)frc/1000.0+(float)dec;	
}

+(float) getTemp:(UInt32Char_t)key{
	SMCVal_t val;
	SMCReadKey2(key, &val, conn);
	return ((val.bytes[0]<<8+val.bytes[1]>>2)>>6);
}

/*+(int)countGpuTemps{
	int i, c=0;
	UInt32Char_t key;
	for (i=0; i<2; i++) {
		sprintf(key, "TG%dD", i);
		if ([smcWrapper getTemp:key]>0)
			c++;
		sprintf(key, "TG%dH", i);
		if ([smcWrapper getTemp:key]>0)
			c++;
		sprintf(key, "TG%dP", i);
		if ([smcWrapper getTemp:key]>0)
			c++;
	}
	return c;
}*/

+(int)countGpus{
	int i, c=0;
	UInt32Char_t key;
	for (i=0; i<2; i++) {
		sprintf(key, "TG%dD", i);
		if ([smcWrapper getTemp:key]>0) {
			c++;
			continue;
		}
		sprintf(key, "TG%dH", i);
		if ([smcWrapper getTemp:key]>0) {
			c++;
			continue;
		}
		sprintf(key, "TG%dP", i);
		if ([smcWrapper getTemp:key]>0) {
			c++;
			continue;
		}
	}
	return c;
	
}

+(int) get_maintemp:(int)core_number{
	UInt32Char_t  key;
	//SMCVal_t      val;
	//kern_return_t result;
	float c_temp;
	
	sprintf(key, "TC%dD", core_number);
	c_temp=[smcWrapper getTemp:key];
	//workaround for imac 24" (just for testing).
	if (c_temp<=0) {
		sprintf(key, "TC%dH", core_number);
		c_temp=[smcWrapper getTemp:key];
	}
	//last try
	if (c_temp<=0) {
		sprintf(key, "TC%cH", 'A'+core_number);
		c_temp=[smcWrapper getTemp:key];
	}
	
	//for macpro different strategy
	
	return c_temp;
}

+(int)countCores{
	int i, c=0, t;
	for (i=0; i<8; i++){
		t=[smcWrapper get_maintemp:i];
		if (t>0)
			c++;
	}
	return c;
}

+(int) get_fan_rpm:(int)fan_number{
	UInt32Char_t  key;
	SMCVal_t      val;
	//kern_return_t result;
	sprintf(key, "F%dAc", fan_number);
	SMCReadKey2(key, &val,conn);
	int running= _strtof(val.bytes, val.dataSize, 2);
	return running;
}	

+(int) get_fan_num{
//	kern_return_t result;
    SMCVal_t      val;
    int           totalFans;
	SMCReadKey2("FNum", &val,conn);
    totalFans = _strtoul(val.bytes, val.dataSize, 10); 
	return totalFans;
}

+(NSString*) get_fan_descr:(int)fan_number{
	UInt32Char_t  key;
	char temp;
	SMCVal_t      val;
	//kern_return_t result;
	NSMutableString *desc;
//	desc=[[NSMutableString alloc] initWithFormat:@"Fan #%d: ",fan_number+1];
	desc=[[[NSMutableString alloc]init] autorelease];
	sprintf(key, "F%dID", fan_number);
	SMCReadKey2(key, &val,conn);
	int i;
	for (i = 0; i < val.dataSize; i++) {
		if ((int)val.bytes[i]>32) {
			temp=(unsigned char)val.bytes[i];
			[desc appendFormat:@"%c",temp];
		}
	}	
	return desc;
}	


+(int) get_min_speed:(int)fan_number{
	UInt32Char_t  key;
	SMCVal_t      val;
	//kern_return_t result;
	sprintf(key, "F%dMn", fan_number);
	SMCReadKey2(key, &val,conn);
	int min= _strtof(val.bytes, val.dataSize, 2);
	return min;
}	

+(int) get_max_speed:(int)fan_number{
	UInt32Char_t  key;
	SMCVal_t      val;
	//kern_return_t result;
	sprintf(key, "F%dMx", fan_number);
	SMCReadKey2(key, &val,conn);
	int max= _strtof(val.bytes, val.dataSize, 2);
	return max;
}	

/*+(NSString*)createCheckSum:(NSString*)path{
	NSData *d=[NSData dataWithContentsOfMappedFile:path];
	unsigned char buffer[EVP_MAX_MD_SIZE];
	unsigned int size=EVP_MAX_MD_SIZE;
	EVP_Digest((void *)[d bytes],[d length],buffer,&size,EVP_sha1(),NULL);
	NSMutableString *sha1=[[[NSMutableString alloc]init] autorelease];
	int i;
	for (i = 0; i < size; i++) {
			[sha1 appendFormat:@"%x",(unsigned char)buffer[i]];
	}		
	return sha1;
}*/

//call smc binary with setuid rights and apply
+(void)setKey_external:(NSString *)key value:(NSString *)value{
	//NSLog(@"smcFanControl: The value: %@ key:%@",value,key);
	NSString *launchPath = [[NSBundle mainBundle]   pathForResource:@"smc" ofType:@""];
	//first check if it's the right binary (security)
	//NSString *checksum=[smcWrapper createCheckSum:launchPath];
	/*if (![checksum  isEqualToString:smc_checksum]) {
		//NSLog(@"smcFanControl: Security Error: smc-binary is not the distributed one");
		return;
	}*/
    NSArray *argsArray = [NSArray arrayWithObjects: @"-k",key,@"-w",value,nil];
	NSTask *task;
    task = [[NSTask alloc] init];
	[task setLaunchPath: launchPath];
	[task setArguments: argsArray];
	[task launch];
	[task release];
}

@end
