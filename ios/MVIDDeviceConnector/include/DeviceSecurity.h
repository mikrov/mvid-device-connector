//
//  DeviceSecurity.h
//  MVIDDeviceConnector
//
//  Created by Jakob on 05/02/13.
//
//

#import <Foundation/Foundation.h>

@class LoginViewController;

/**
 DeviceSecurity is the client side interface towards the corresponding MVID seb service <a href="https://signon.mv-nordic.com/device-security/DeviceSecurity">DeviceSecurity</a>. It is designed for easy integration into existing iOS apps.
 Once the interface is properly integrated into an app it will provide:
    - MVID User authentication
    - Product access services
    - MVID Session ID for further requesting into MVID's web services: <a href="https://mvid-services.mv-nordic.com/v2">MVID Services</a>
 
 \section usage Usage
 The primary method of DeviceSecurity is the \link DeviceSecurity.doLogin:parentViewController:altSubView: doLogin:\endlink method. In a normal
 integration this method should be all that is needed.
 
 The basic integration has 3 steps:
    - Instantiate DeviceSecurity as a member of the class responsible for the login process.
    - Add a notification observer to listen for the final asynchronious login response "MVIDLoginResponseReady"
    - Call \link DeviceSecurity.doLogin:parentViewController:altSubView: doLogin:\endlink

 \subsection MVIDLoginResponseReady MVIDLoginResponseReady notification object
 The MVIDLoginResponseReady notification is posted via the default notification center with a NSObject as userInfo (user data).
 The posted userInfo defines the following attributes:
 
 First Header  | Second Header    | Description
 ------------- | ---------------- | -----------------------------------------------
 NSNumber      | service_result   | Bitwise combination of ServiceResult flags
 NSNumber      | has_access       | 1 if the access_identifier is granted 0 if not
 NSNumber      | service_res_code | The server side result code
 NSString      | service_res_msg  | The server side result message
 
 \endsubsection

 \subsection example Coding example
 
 The following is a ver simple example of how a UIViewController with MVID Login responsibility could be coded.

 AppLoginViewController.h:
 @code
 
    // Very basic header
    #import <UIKit/UIKit.h>
    @class DeviceSecurity;
    @interface AppLoginViewController : UIViewController {
        DeviceSecurity *device_security;
        NSString *cur_mv_session_id;
    }

    @property(copy) NSString *cur_mv_session_id;

    @end

 @endcode

 AppLoginViewController.mm:
 @code
 
    #import "AppLoginViewController.h"
    #import "DeviceSecurity.h"
    @interface AppLoginViewController ()
    @end

    @implementation AppLoginViewController

    @synthesize cur_mv_session_id = _cur_mv_session_id;


    // Initializing MVID DeviceSecurity
    - (void)viewDidLoad
    {
        // Custom initialization
        device_security = [[DeviceSecurity alloc] init];
        // Some pre-configurations (only want school logins)
        [device_security excludeLoginGroup:@"private"];
        [device_security excludeLoginGroup:@"company"];
        // Start observing notifications named "MVIDLoginResponseReady"
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loginResponse:) name:@"MVIDLoginResponseReady" object:nil];
        [super viewDidLoad];
    }


    // Notification callback method
    - (void) loginResponse:(NSNotification *) notification {
        // Extract interesting notification values
        int service_result = [[notification.userInfo valueForKey:@"service_result"] intValue];
        BOOL has_access = [[notification.userInfo valueForKey:@"has_access"] boolValue];
        int service_res_code = [[notification.userInfo valueForKey:@"service_res_code"] intValue];
        NSString *service_res_msg = [notification.userInfo valueForKey:@"service_res_msg"];
        // copy MVID Session ID into a class local property.
        cur_mv_session_id = device_security.mv_session_id;
        // Just log some results
        NSLog(@"service_res_code: %d", service_res_code);
        NSLog(@"service_res_msg: %@", service_res_msg);
        NSLog(@"mv_session_id: %@", cur_mv_session_id);
        NSLog(@"Access granted: %d", has_access);
        NSLog(@"service_result: %d", service_result);
    }


    // Something triggered the MVID Login process (in this case a button was touched)
    - (IBAction)startLogin:(id)sender {
        [device_security doLogin:@"product.ios.ml.myapp"
            parentViewController:self];
    }

    @end

 @endcode

 \endsubsection
 \endsection
 
 */
@interface DeviceSecurity : NSObject {
    NSString *mv_session_id;
    NSString *access_identifier;
    NSString *device_hash;
    NSNumber *device_borrow_time;
    NSError *network_error;
    int service_res_code;
    NSString *service_res_msg;
    LoginViewController *login_vc;
}

/**
 Following values apply to the <a href="https://signon.mv-nordic.com/device-security/DeviceSecurity">DeviceSecurity</a> web service.
 In the descriptions below it is specified which methods in the DeviceSecurity class that can recieve which results directly.
 
 When using \link DeviceSecurity.doLogin:parentViewController:altSubView: doLogin:\endlink both DeviceSecurity.registerDevice: and
 DeviceSecurity.applicationLogin: are candidates for being called, therefore all ServiceResult codes are bitwise candidates of the
 final login result, which is posted in the \ref MVIDLoginResponseReady via the default notification center.
 
 Read more about Apple's notification system here: <a href="https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/Notifications/Introduction/introNotifications.html#//apple_ref/doc/uid/10000043-SW1">Notification Programming Topics</a>.
 */
enum ServiceResult {
    /**
     Successful operation.
        - DeviceSecurity.registerDevice:
            - registerDevice method has been succesfully queried.
        - DeviceSecurity.applicationLogin:
            - The registered user has succesfully queried the applicationLogin method */
    Success = 1,
    /**
    Invalid MV Session ID.
        - DeviceSecurity.registerDevice:
            - The mv_session_id passed to registerDevice is invalid. Obvious reason
              would be that it is too old and therefore timed out on the server side. */
    InvalidMVSessionID = 2,
    /**
     Application access denied.
        - DeviceSecurity.applicationLogin:
            - The server was contacted and device hash is good, but the specific user
              mapped to the device does not have access to the application. */
    AccessDenied = 4,
    /**
     Application borrow-time has expired.
        - DeviceSecurity.applicationLogin:
            - Either the device or the MV-ID server is telling you that the time since
              last applicationLogin has expired the borrow-time allowed for the
              application. */
    ApplicationBorrowTimeExpired = 8,
    /**
    Device borrow-time has expired.
        - DeviceSecurity.applicationLogin:
            - Either the device or the MV-ID server is telling you that the time since
              last applicationLogin has expired the borrow-time allowed for the
              device registration. This meens that the device is no longer mapped to a
              user on MV-ID and a login is requiered.
     
        \note If device has no device hash registered this ServiceResult is also returned.*/
    DeviceBorrowTimeExpired = 16,
    /**
     Fault occured while calling the Device Security service.
        - DeviceSecurity.registerDevice:
        - DeviceSecurity.applicationLogin:
            - Some fault occured during the invocation of a service method. Info
              about the fault is available in the response object itself. */
    ServiceFault = 32,
    /**
     ServiceSuccess meens that the a service method invocation succeeded with no
     faults. This does not necessarily meen that a device was registered succesfully
     or that access was granted to an application successfully.
     */
    ServiceSuccess = 64,
    /**
     Network error occured.
        - DeviceSecurity.registerDevice:
        - DeviceSecurity.applicationLogin:
            - A network transport error has occured. Probably no link or other
              kind of network related problem like missing DNS service. Service
              requests timeout after 2 seconds, so connection problems should
              not "hang" the application. */
    NetworkError = 128
};

@property(copy) NSString *mv_session_id;
@property(copy) NSString *access_identifier;
@property(copy) NSString *device_hash;
@property(copy) NSNumber *device_borrow_time;
@property(copy) NSError *network_error;
@property(copy) NSString *service_res_msg;
@property int service_res_code;

/** \cond */
- (void)readDeviceHash;
- (int)postRequest:(NSString*)params responseDictionary:(NSDictionary**)resp_dict resultCode:(int*)res_code;
- (void)postMVIDResponseReady:(BOOL)has_access service_result:(int)res;
/** \endcond */


/**
 Register a device at MVID with a valid mv_session_id using <a href="https://signon.mv-nordic.com/device-security/DeviceSecurity">DeviceSecurity</a> web service.
 
 It should not be necessary to call this method from a third-party application since it
 is automatically invoked from \link DeviceSecurity.doLogin:parentViewController:altSubView: doLogin\endlink if needed.
 
 @param mv_session_id
     The MVID session ID which should be associated with the device.
 
 @return bitwise combination of DeviceSecurity.ServiceResult
*/
- (int)registerDevice:(NSString*)mv_session_id;
/**
 Register the device for application usage. This method will only succeed on a properly registered device (see DeviceSecurity.registerDevice:).
 
 It should not be necessary to call this method from a third-party application since it
 is automatically invoked from DeviceSecurity:doLogin: if needed.
 
 @param access_identifier
     The access_identifier identifying the application being started.
 
 @return bitwise combination of DeviceSecurity.ServiceResult
 */
- (int)applicationLogin:(NSString*)access_identifier;
/**
 Start the login process, communicating with MVID backend servers if online, else if any previous login has been performed
 and is still within the "borrow time" grace period, this will be used directly off the device. This means that a single
 login operation can be reused in offline mode some time.
 
 \note
    * To force device to re-login you must call resetDeviceRegistraion first and be sure to pass a parentViewController.
    * To direct the login screen to a subview in your view controller use the altSubView parameter.

 Example usage:
 @code
 // In this example the LoginViewController's views are displayed in a
 // overlay subview of my LoginController called myLoginView
 BOOL login_view_displayed = 
    [device_security doLogin:current_access_identifier
        parentViewController:self
                  altSubView:self.myLoginView];
 if (login_view_displayed) {
    // display and fade in my overlay view
    self.myLoginView.hidden = NO;
    [UIView animateWithDuration:0.25
                     animations:^{self.myLoginView.alpha = 1.0;}];
 }
 @endcode

 
 @param access_identifier
     The access_identifier that needs to be checked.
 @param parentViewController
     The visible part of the login process is controlled by the LoginViewController which should be owned by a UIViewController
     in the applications domain. Therefore a parent UIViewController is mandatory in order to display the login UI. It is legal
     to pass nil if you just want to check if the user already has access and need to know that the login UI is not shown.
 @param altSubView
     If nil is passed the then the LoginViewController's root UIView will be a sub view of the parentViewController's root UIView.
     By passing an alternate UIView you can control where the login UI should be displayed in your UIViewController's view
     hierarchi.
 @return (Has the login view been displayed)
     This value does *NOT* state if the login was a success or not, the actual result of the login is sent via the
     notificationCenter in the MVIDLoginResponseReady notification. The return value only says whether or not a login view has been
     displayed. For example:
        - If for instance the user calls doLogin with a nil parentViewController, and the device is not
          registered, then doLogin cannot display the Login UI. This means that FALSE is returned and a
          MVIDLoginResponseReady notification can be posted right away telling the calling function that
          the login gave no access.
        - If a device is not registered (or has not been online for a while) and a parentViewController is passed then
          the LoginViewController is displayed and TRUE is returned.
 */
- (BOOL) doLogin:(NSString *)access_identifier parentViewController:(UIViewController *)vc altSubView:(UIView*)alt_sub_view;
/**
 Start the login process. This is a convenience method that calls \link DeviceSecurity.doLogin:parentViewController:altSubView: doLogin\endlink
 with alt_sub_view as nil. This means that the parentViewController's root view will be used as parent view for the login UI.

 Example usage:
 @code
 // In this example LoginViewController's view are placed on top
 // of the parent view controller's root view.
 BOOL login_view_displayed =
    [device_security doLogin:current_access_identifier
        parentViewController:self];
 @endcode
 
 @param access_identifier
    The access_identifier that needs to be checked.
 @param parentViewController
    The visible part of the login process is controlled by the LoginViewController which should be owned by a UIViewController
    in the applications domain. Therefore a parent UIViewController is mandatory in order to display the login UI. It is legal
    to pass nil if you just want to check if the user already has access and need to know that the login UI is not shown.
 @return (Has the login view been displayed)
    This value does *NOT* state if the login was a success or not, the actual result of the login is sent via the
    notificationCenter in the MVIDLoginResponseReady notification. The return value only says whether or not a login view has been
    displayed. For example:
        - If for instance the user calls doLogin with a nil parentViewController, and the device is not
          registered, then doLogin cannot display the Login UI. This means that FALSE is returned and a
          MVIDLoginResponseReady notification can be posted right away telling the calling function that
          the login gave no access.
        - If a device is not registered (or has not been online for a while) and a parentViewController is passed then
          the LoginViewController is displayed and TRUE is returned.

*/
- (BOOL) doLogin:(NSString *)access_identifier parentViewController:(UIViewController *)vc;
/**
 Move the center point of the login UI n pixels to the left or right depending on the numeric sign.

 Example usage:
 @code
    // Horizontally displace the login 40 pixels to the left
    [device_security setLoginCenterDisplacement:-40];
 @endcode
 
 @param pixels
    The amount of pixels to displace the center with.
 */
- (void) setLoginCenterDisplacement:(int)pixels;
/**
 Exclude a login group from the login UI.
 
 Example usage:
 @code
    // Exclude the school login group
    [device_security excludeLoginGroup:@"company"];
 @endcode
 
 @param login_group
    The name of the login group. At the time of writing possible groups are: school, company and private
 */
- (void) excludeLoginGroup:(NSString *)login_group;
/**
 Re-include all login groups. Use this if you have already excluded some login groups and want to include them all again
 without re-instantiating DeviceSecurity (which is discouraged)
 
 Example usage:
 @code
    // Include all login groups
    [device_security includeAllLoginGroups];
 @endcode
 */
- (void) includeAllLoginGroups;
/** \cond */
- (void) setApplicationCountryCode:(NSString*)applicationCountryCode;
/** \endcond */

/**
 Release the device-side knowledge about MVID within the app's sandbox forcing a new login from the user.
 
 Calling this method will not affect other apps using MVIDDeviceConnector.
 
 Example usage:
 @code
     // Release device-side knowlegde of MVID
     [device_security releaseDeviceRegistration];
 @endcode
 
 */
- (void) releaseDeviceRegistration;
/**
 Release the device-side knowledge about whether or not access has been granted to an application. This
 will force the application to do a server-side check next time DeviceSecurity.doLogin is used.
 
 This will not throw away the device registration, so re-login by user is not needed next time the device
 is online.
 
 Example usage:
 @code
     // Forget whether access is granted or not to MyApp
     [device_security releaseApplicationUsage:@"products.ios.ml.myapp"]ß	;
 @endcode
 
 @param pixels
    The amount of pixels to displace the center with.
 */
- (void) releaseApplicationUsage:(NSString* )access_identifier;
@end
