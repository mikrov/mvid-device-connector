//
//  AppLoginViewController.m
//  TestMVID
//
//  Created by Jakob Simon-Gaarde on 19/03/14.
//  Copyright (c) 2014 Home. All rights reserved.
//

#import "AppLoginViewController.h"
#import "DeviceSecurity.h"

@interface AppLoginViewController ()

@end

@implementation AppLoginViewController
@synthesize aiSelect = _aiSelect;
@synthesize accessIdentifiers = _accessIdentifiers;
@synthesize current_access_identifier = _current_access_identifier;

- (void)viewDidLoad
{
    accessIdentifiers = [[NSArray alloc] initWithObjects:
                         @"product.mac.ml.intowords",
                         @"product.web.da.10fingers",
                         @"product.web.da.intowords",
                         @"product.web.sv.intowords",
                         @"product.ios.ml.intowords",
                         @"product.web.da.mathinmoonville", nil];
    current_access_identifier = [accessIdentifiers objectAtIndex:0];
    
    // Custom initialization
    device_security = [[DeviceSecurity alloc] init];
    [device_security setLoginCenterDisplacement:0];
    [device_security excludeLoginGroup:@"private"];
    [device_security excludeLoginGroup:@"company"];
    [device_security setApplicationCountryCode:@"IntoWords_dk"];
    
    self.lView.clipsToBounds = YES;
    self.lView.hidden = YES;
    self.lView.alpha = 0;
    
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (NSInteger) numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    NSLog(@"numberOfRows: %lu",(unsigned long)[accessIdentifiers count]);
    return [accessIdentifiers count];
}

- (NSString *) pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return [accessIdentifiers objectAtIndex:row];
}

-(void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    current_access_identifier = [accessIdentifiers objectAtIndex:row];
    [self.lab_session_id setText:@""];
    [self.lab_res_code setText:@""];
    [self.lab_res_msg setText:@""];
}

- (void)viewDidUnload {
    [self setLab_session_id:nil];
    [self setAiSelect:nil];
    [self setLab_res_code:nil];
    [self setLab_res_msg:nil];
    [super viewDidUnload];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)resetDeviceReg:(id)sender {
    [device_security releaseDeviceRegistration];
}

- (IBAction)resetAppReg:(id)sender {
    [device_security releaseApplicationUsage:current_access_identifier];
}

- (IBAction)doLogin:(id)sender {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loginResponse:) name:@"MVIDLoginResponseReady" object:nil];
    [device_security doLogin:current_access_identifier parentViewController:nil];
}

- (IBAction)doLoginWithView:(id)sender {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loginResponse:) name:@"MVIDLoginResponseReady" object:nil];
    if ([device_security doLogin:current_access_identifier
            parentViewController:self
                      altSubView:self.lView]) {
        self.lView.hidden = NO;
        [UIView animateWithDuration:0.25
                         animations:^{
                             self.lView.alpha = 1.0;
                         }];
    }
}


- (void) loginResponse:(NSNotification *) notification {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"MVIDLoginResponseReady" object:nil];
    BOOL has_access = [[notification.userInfo valueForKey:@"has_access"] boolValue];
    NSNumber *service_res_code = [notification.userInfo valueForKey:@"service_res_code"];
    NSString *service_res_msg = [notification.userInfo valueForKey:@"service_res_msg"];
    [self.lab_session_id setText:(has_access?@"TRUE":@"FALSE")];
    [self.lab_res_code setText:[NSString stringWithFormat: @"%@",service_res_code]];
    [self.lab_res_msg setText:service_res_msg];
    NSLog(@"mv_session_id: %@",device_security.mv_session_id);
    NSLog(@"Access granted: %d",has_access);
    [UIView animateWithDuration:0.25
                     animations:^{self.lView.alpha = 0.0;}
                     completion:^(BOOL finished) {
                         self.lView.hidden = YES;
                     }];
}

@end
