//
//  ViewController.h
//  X_V
//
//  Created by zouxu on 14/6/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "AllDefines.h"
 


@interface ViewController : UIViewController
@property (retain, nonatomic) IBOutlet UIImageView *img;
@property (retain, nonatomic) IBOutlet UIProgressView *progressView;
- (IBAction)choice:(id)sender;
@property (retain, nonatomic) IBOutlet UISwitch *choiced;
@property (retain, nonatomic) IBOutlet UILabel *text;
@property (retain, nonatomic) IBOutlet UILabel *unicode;

@end
