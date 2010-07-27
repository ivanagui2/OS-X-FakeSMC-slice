/*
 *	FanControl
 *
 *	Copyright (c) 2006 Hendrik Holtmann
*
 *	MachineDefaults.m - MacBook(Pro) FanControl application
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

#import "MachineDefaults.h"

BOOL firstCall;
NSDictionary *new_machine;

@implementation MachineDefaults


- (id)init:(NSString*)p_machine{
	machine=[MachineDefaults computerModel];
	supported_machines=[[NSArray alloc] init];
	firstCall=YES;
	return self;
}


-(Boolean)is_supported{
	/*int i;
	supported=NO;
	for(i=0;i<[supported_machines count];i++) {
		if ([machine isEqualToString:[[supported_machines objectAtIndex:i] objectForKey:@"Machine"]]) {
			supported=YES;
			machine_num=i;
			//NSLog(@"Found: %@",[[supported_machines objectAtIndex:i] objectForKey:@"Machine"]);
		}
	}*/
	return YES;
}



-(NSDictionary*) readfrom_plist{
	/*if (!supported) {return nil;}
	return [supported_machines objectAtIndex:machine_num];*/
	return nil;
}



-(NSDictionary*) readfrom_smc{
	if (firstCall) {
		firstCall=NO;
		int num_fans,i;
		[smcWrapper init];	
		num_fans=[smcWrapper get_fan_num];
		NSString  *desc;
		NSNumber *min,*max;
		//NSData *xmldata;
		//NSString *error;
		NSMutableArray *fans=[[NSMutableArray alloc] init];
		for (i = 0; i < num_fans; i++) {
			//desc=[NSString stringWithFormat:@"Fan #%d",i+1];
			//NSLog(@"Desc: %@",desc);
			min=[NSNumber numberWithInt:[smcWrapper get_min_speed:i]];
			//NSLog(@"Min: %d",[smcWrapper get_min_speed:i]);
			max=[NSNumber numberWithInt:[smcWrapper get_max_speed:i]];
			desc=[smcWrapper get_fan_descr:i];
			//NSLog(@"Max: %d",[smcWrapper get_max_speed:i]);
			[fans addObject:[NSDictionary dictionaryWithObjectsAndKeys:desc,@"Description",min,@"Minspeed",max,@"Maxspeed",min,@"selspeed",nil]];
		}
		//save to plist for future
		//NSMutableArray *supported_m=[[NSMutableArray alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Machines" ofType:@"plist"]];
		new_machine= [NSDictionary dictionaryWithObjectsAndKeys:fans,@"Fans",[NSNumber numberWithInt:num_fans],@"NumFans",machine,@"Machine",nil];
		[fans release];
		/*[supported_m addObject:new_machine];
		 
		 //save to plist
		 xmldata = [NSPropertyListSerialization dataFromPropertyList:supported_m
															  format:NSPropertyListXMLFormat_v1_0
													errorDescription:&error];
		 [xmldata writeToFile:[[NSBundle mainBundle] pathForResource:@"Machines" ofType:@"plist"] atomically:YES];
		 [supported_m release];*/
	}
	//return new machine-live-data
	NSLog(@"New Machine: %@\n",new_machine);
	return new_machine;
	
}




-(NSDictionary*)get_machine_defaults{
	NSLog(@"get_mach_def\n");
	NSDictionary *m_defaults=nil;
	if ([self is_supported]) {
		m_defaults=[self readfrom_smc];
		/*int i;
		//localize fan-descriptions
		for (i=0;i<[[m_defaults objectForKey:@"Fans"] count];i++) {
			NSString *newvalue=NSLocalizedString([[[m_defaults objectForKey:@"Fans"] objectAtIndex:i] objectForKey:@"Description"],nil);
			[[[m_defaults objectForKey:@"Fans"] objectAtIndex:i] setValue:newvalue forKey:@"Description"];
		}*/
	} /*else {
		NSAlert *alert = [NSAlert alertWithMessageText:NSLocalizedString(@"Alert!",nil) 
						  defaultButton:NSLocalizedString(@"Continue",nil) alternateButton:NSLocalizedString(@"Quit",nil) otherButton:nil
						informativeTextWithFormat:NSLocalizedString(@"K-Stat-i has not been tested on this machine yet, but it should run if you follow the instructions. \n\nIf you choose to continue, please make you have no other FanControl-software running. Otherwise please quit, deinstall the other software, restart your machine and rerun SMCFanControl!",nil)];
		int code=[alert runModal];
		if (code==NSAlertDefaultReturn) {
			m_defaults=[self readfrom_smc];
		} else {
			[[NSApplication sharedApplication] terminate:nil];
		}
		
	}*/
	return m_defaults;
}

+ (NSString *)computerModel
{
    static NSString *computerModel = nil;
    if (!computerModel) {
        io_service_t pexpdev;
        if ((pexpdev = IOServiceGetMatchingService (kIOMasterPortDefault, IOServiceMatching("IOPlatformExpertDevice"))))
        {
            NSData *data;
            if ((data = (id)IORegistryEntryCreateCFProperty(pexpdev, CFSTR("model"), kCFAllocatorDefault, 0))) {
                computerModel = [[NSString allocWithZone:NULL]  initWithCString:[data bytes] encoding:NSASCIIStringEncoding];
                [data release];
            }
        }
    }
    return computerModel;
}

- (void)dealloc{
	[super dealloc];
	//[supported_machines release];
}

@end
