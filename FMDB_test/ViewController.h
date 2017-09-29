//
//  ViewController.h
//  FMDB_test
//
//  Created by Risen on 2017/9/29.
//  Copyright © 2017年 Risen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
@property (weak, nonatomic) IBOutlet UITextField *nameTextfield;

@property (weak, nonatomic) IBOutlet UITextField *sexTextfield;
@property (weak, nonatomic) IBOutlet UITextField *ageTextfield;
- (IBAction)save:(id)sender;
- (IBAction)query:(id)sender;
- (IBAction)queryByCondition:(id)sender;
@property (weak, nonatomic) IBOutlet UIButton *update;
- (IBAction)update:(id)sender;
- (IBAction)deleteByContidion:(id)sender;

@end

