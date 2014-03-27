//
//  AppLoginViewController.h
//  TestMVID
//
//  Created by Jakob Simon-Gaarde on 19/03/14.
//  Copyright (c) 2014 Home. All rights reserved.
//

#import <UIKit/UIKit.h>

@class DeviceSecurity;

@interface AppLoginViewController : UIViewController
<UIPickerViewDelegate, UIPickerViewDataSource> {
    UIPickerView   *aiSelect;
    NSArray        *accessIdentifiers;
    DeviceSecurity *device_security;
    NSString       *current_access_identifier;
}

@property (nonatomic, retain) IBOutlet UIPickerView *aiSelect;
@property(copy) NSString *current_access_identifier;
@property (nonatomic, retain) NSArray *accessIdentifiers;
@property (strong, nonatomic) IBOutlet UILabel *lab_session_id;
@property (strong, nonatomic) IBOutlet UILabel *lab_res_code;
@property (strong, nonatomic) IBOutlet UILabel *lab_res_msg;
@property (strong, nonatomic) IBOutlet UIView *lView;

- (IBAction)resetDeviceReg:(id)sender;
- (IBAction)resetAppReg:(id)sender;
- (IBAction)doLogin:(id)sender;
- (IBAction)doLoginWithView:(id)sender;

@end
