＃FMDB_test
这是一个学习测试FMDBdemo 
本文主要从以下几个方面介绍FMDB,保存,查询,条件查询,更新,条件删除来介绍

1.简介

FMDB是iOS平台的SQLite数据库框架，它是以OC的方式封装了SQLite的C语言API，它相对于cocoa自带的C语言框架有如下的优点:

使用起来更加面向对象，省去了很多麻烦、冗余的C语言代码

对比苹果自带的Core Data框架，更加轻量级和灵活

提供了多线程安全的数据库操作方法，有效地防止数据混乱
2.核心类

FMDB有三个主要的类：

FMDatabase
一个FMDatabase对象就代表一个单独的SQLite数据库，用来执行SQL语句

FMResultSet
使用FMDatabase执行查询后的结果集

FMDatabaseQueue
用于在多线程中执行多个查询或更新，它是线程安全的
3.打开数据库和c语言框架一样，FMDB通过指定SQLite数据库文件路径来创建FMDatabase对象，但FMDB更加容易理解，使用起来更容易，使用之前一样需要导入sqlite3.dylib。打开数据库,创建表(也叫更新)的方法如下：

   //获取Document文件夹下的数据库文件，没有则创建
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *dbPath = [docPath stringByAppendingPathComponent:@"user.db"];
    NSLog(@"dbPath = %@",dbPath);
    //获取数据库并打开
    FMDatabase *dataBase = [FMDatabase databaseWithPath:dbPath];
    if (![dataBase open]) {
        NSLog(@"打开数据库失败");
        return ;
    }


 //创建表（FMDB中只有update和query操作，除了查询其他都是update操作）
    [dataBase executeUpdate:@"create table if not exists user(name text,gender text,age integer) "];

插入数据的操作:

//常用方法有以下3种：   
/* 执行更新的SQL语句，字符串里面的"?"，依次用后面的参数替代，必须是对象，不能是int等基本类型 */
- (BOOL)executeUpdate:(NSString *)sql,... ;
/* 执行更新的SQL语句，可以使用字符串的格式化进行构建SQL语句 */
- (BOOL)executeUpdateWithFormat:(NSString*)format,... ;
/* 执行更新的SQL语句，字符串中有"?"，依次用arguments的元素替代 */
- (BOOL)executeUpdate:(NSString*)sql withArgumentsInArray:(NSArray *)arguments;


/* 1. 直接使用完整的SQL更新语句 */
[database executeUpdate:@"insert into mytable(num,name,sex) values(0,'liuting','m');"];

NSString *sql = @"insert into mytable(num,name,sex) values(?,?,?);";
/* 2. 使用不完整的SQL更新语句，里面含有待定字符串"?"，需要后面的参数进行替代 */
[database executeUpdate:sql,@0,@"liuting",@"m"];
/* 3. 使用不完整的SQL更新语句，里面含有待定字符串"?"，需要数组参数里面的参数进行替代 */
[database executeUpdate:sql 
   withArgumentsInArray:@[@0,@"liuting",@"m"]];

/* 4. SQL语句字符串可以使用字符串格式化，这种我们应该比较熟悉 */
[database executeUpdateWithFormat:@"insert into mytable(num,name,sex) values(%d,%@,%@);",0,@"liuting","m"];

本demo采用第二种方法

 //常用方法有以下3种：
    //    - (BOOL)executeUpdate:(NSString*)sql, ...
    //    - (BOOL)executeUpdateWithFormat:(NSString*)format, ...
    //    - (BOOL)executeUpdate:(NSString*)sql withArgumentsInArray:(NSArray *)arguments


    //插入数据
    BOOL inser = [dataBase executeUpdate:@"insert into user values(?,?,?)",_nameTextField.text,_sexTextField.text,_ageTextField.text];


    if (inser) {
        UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"提示" message:@"信息保存成功" delegate:self cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        [alert show];
    }
    [dataBase close];

5.查询

查询方法也有3种，使用起来相当简单：

// 全部查询
- (FMResultSet *)executeQuery:(NSString*)sql, ...
// 条件查询
- (FMResultSet *)executeQueryWithFormat:(NSString*)format, ...
- (FMResultSet *)executeQuery:(NSString *)sql withArgumentsInArray:(NSArray *)arguments
// 演示如下:

//1.执行查询
FMResultSet *result = [database executeQuery:@"SELECT * FROM t_person"];
//2.遍历结果集
while ([result next]) {
    NSString *name = [result stringForColumn:@"name"];
    int age = [result intForColumn:@"age"];
}
/*
FMResultSet获取不同数据格式的方法

/* 获取下一个记录 */
- (BOOL)next;
/* 获取记录有多少列 */
- (int)columnCount;
/* 通过列名得到列序号，通过列序号得到列名 */
- (int)columnIndexForName:(NSString *)columnName;
- (NSString *)columnNameForIndex:(int)columnIdx;
/* 获取存储的整形值 */
- (int)intForColumn:(NSString *)columnName;
- (int)intForColumnIndex:(int)columnIdx;
/* 获取存储的长整形值 */
- (long)longForColumn:(NSString *)columnName;
- (long)longForColumnIndex:(int)columnIdx;
/* 获取存储的布尔值 */
- (BOOL)boolForColumn:(NSString *)columnName;
- (BOOL)boolForColumnIndex:(int)columnIdx;
/* 获取存储的浮点值 */
- (double)doubleForColumn:(NSString *)columnName;
- (double)doubleForColumnIndex:(int)columnIdx;
/* 获取存储的字符串 */
- (NSString *)stringForColumn:(NSString *)columnName;
- (NSString *)stringForColumnIndex:(int)columnIdx;
/* 获取存储的日期数据 */
- (NSDate *)dateForColumn:(NSString *)columnName;
- (NSDate *)dateForColumnIndex:(int)columnIdx;
/* 获取存储的二进制数据 */
- (NSData *)dataForColumn:(NSString *)columnName;
- (NSData *)dataForColumnIndex:(int)columnIdx;
/* 获取存储的UTF8格式的C语言字符串 */
- (const unsigned cahr *)UTF8StringForColumnName:(NSString *)columnName;
- (const unsigned cahr *)UTF8StringForColumnIndex:(int)columnIdx;
/* 获取存储的对象，只能是NSNumber、NSString、NSData、NSNull */
- (id)objectForColumnName:(NSString *)columnName;
- (id)objectForColumnIndex:(int)columnIdx;
*/

本文demo中代码演示:

//查询全部
- (IBAction)query:(id)sender {
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *dbPath = [docPath stringByAppendingPathComponent:@"user.db"];

    FMDatabase *dataBase = [FMDatabase databaseWithPath:dbPath];
    if (![dataBase open]) {
        NSLog(@"打开数据库失败");
        return ;
    }
    FMResultSet *resultSet = [dataBase executeQuery:@"select * from user"];
    while ([resultSet next]) {
        NSString *name = [resultSet stringForColumn:@"name"];
        NSString *genter = [resultSet stringForColumn:@"gender"];
        int age = [resultSet intForColumn:@"age"];
        NSLog(@"Name:%@,Gender:%@,Age:%d",name,genter,age);
    }

    [dataBase close];
}
//条件查询
- (IBAction)queryByCondition:(id)sender {
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *dbPath = [docPath stringByAppendingPathComponent:@"user.db"];
    FMDatabase *dataBase = [FMDatabase databaseWithPath:dbPath];
    if (![dataBase open]) {
        return ;
    }
//    FMResultSet *resultSet = [dataBase executeQuery:@"select *from user where name = ?",@"ZY"];
    FMResultSet *resultSet = [dataBase executeQueryWithFormat:@"select * from user where name = %@",@"zy"];
    while ([resultSet next]) {
        NSString *name = [resultSet stringForColumnIndex:0];
        NSString *gender = [resultSet stringForColumn:@"gender"];
        int age = [resultSet intForColumn:@"age"];
        NSLog(@"Name:%@,Gender:%@,Age:%d",name,gender,age);
    }
    [dataBase close];
}

6 :条件删除的方法

- (IBAction)deleteByCondition:(id)sender
{
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *dbPath = [docPath stringByAppendingPathComponent:@"user.db"];
    FMDatabase *dataBase = [FMDatabase databaseWithPath:dbPath];
    if (![dataBase open]) {
        return ;
    }
    BOOL delete = [dataBase executeUpdateWithFormat:@"delete from user where name = %@",@"zy"];
    if (delete) {
        UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"提示" message:@"信息删除成功" delegate:self cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        [alert show];
    }
    [dataBase close];
}

7.线程安全

产考使用FMDB事务批量更新数据库速度问题里面的代码进行使用

在多个线程中同时使用一个FMDatabase实例是不明智的。不要让多个线程分享同一个FMDatabase实例，它无法在多个线程中同时使用。 如果在多个线程中同时使用一个FMDatabase实例，会造成数据混乱等问题。所以，请使用 FMDatabaseQueue，它是线程安全的。以下是使用方法：
1. 创建

NSString *path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES).firstObject; 
NSString *filePath = [path stringByAppendingPathComponent:@"FMDB.db"];
FMDatabaseQueue *queue = [FMDatabaseQueue databaseQueueWithPath:path];

2. 操作数据库

[queue inDatabase:^(FMDatabase*db) {
    //FMDatabase数据库操作
}];

**3.本文的使用实例

- (IBAction)save:(id)sender {
    //获取Document文件夹下的数据库文件，没有则创建
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *dbPath = [docPath stringByAppendingPathComponent:@"user.db"];
    NSLog(@"dbPath = %@",dbPath);
    //获取数据库并打开
  //  FMDatabase *dataBase = [FMDatabase databaseWithPath:dbPath];

    //多线程安全FMDatabaseQueue可以替代dataBase
    FMDatabaseQueue *dataBasequeue = [FMDatabaseQueue databaseQueueWithPath:dbPath];

    [dataBasequeue inDatabase:^(FMDatabase *db) {


        if (![db open]) {
            NSLog(@"打开数据库失败");
            return ;
        }
        //创建表（FMDB中只有update和query操作，除了查询其他都是update操作）
        [db executeUpdate:@"create table if not exists user(name text,gender text,age integer) "];

        //常用方法有以下3种：
        //    - (BOOL)executeUpdate:(NSString*)sql, ...
        //    - (BOOL)executeUpdateWithFormat:(NSString*)format, ...
        //    - (BOOL)executeUpdate:(NSString*)sql withArgumentsInArray:(NSArray *)arguments


        //插入数据
        BOOL inser = [db executeUpdate:@"insert into user values(?,?,?)",_nameTextField.text,_sexTextField.text,_ageTextField.text];
        if (inser) {
            UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"提示" message:@"信息保存成功" delegate:self cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
            [alert show];
        }
        [db close];
    }];
}

7:而且可以轻松地把简单任务包装到事务里：
之所以将事务放到FMDB中去说并不是因为只有FMDB才支持事务，而是因为FMDB将其封装成了几个方法来调用，不用自己写对应的SQL而已,假如你要对数据库中的Stutent表插入新数据，那么该事务的具体过程是：开始新事物->插入数据->提交事务，那么当我们要往该表内插入500条数据，如果按常规操作处理就要执行500次“开始新事物->插入数据->提交事务”的过程。

好吧，今天的重点来了,举个例子:假如北京的一家A工厂接了上海一家B公司的500件产品的订单，思考一下：A工厂是生产完一件立即就送到B公司还是将500件产品全部生产完成后再送往B公司？答案肯定是后者，因为前者浪费了大量的时间、人力物力花费在往返于北京和上海之间。同样这个道理也能用在我们的数据库操作上，下面是我自己对使用事务和不使用事务的两种测试：
SQLite进行事务的SQL语句：

只要在执行SQL语句前加上以下的SQL语句，就可以使用事务功能了：
开启事务的SQL语句，"begin transaction;"
进行提交的SQL语句，"commit transaction;"
进行回滚的SQL语句，"rollback transaction;"

一: FMDatabase使用事务的方法：

-(void)transaction {
    NSDate *date1 = [NSDate date];

// 创建表
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *dbPath = [docPath stringByAppendingPathComponent:@"mytable1.db"];
    NSLog(@"dbPath = %@",dbPath);
    FMDatabase *dataBase = [FMDatabase databaseWithPath:dbPath];

    // 注意这里的判断一步都不能少,特别是这里open的判断

    if (![dataBase open]) {
        NSLog(@"打开数据库失败");
        return ;
    }

    NSString *sqlStr = @"create table if not exists mytable1(num integer,name varchar(7),sex char(1),primary key(num));";
    BOOL res = [dataBase executeUpdate:sqlStr];
    if (!res) {
        NSLog(@"error when creating mytable1");

        [dataBase close];
    }

    // 开启事务
    [dataBase beginTransaction];
    BOOL isRollBack = NO;
    @try {
        for (int i = 0; i<500; i++) {
            NSNumber *num = @(i+1);
            NSString *name = [[NSString alloc] initWithFormat:@"student_%d",i];
            NSString *sex = (i%2==0)?@"f":@"m";

            NSString *sql = @"insert into mytable1(num,name,sex) values(?,?,?);";
            BOOL result = [dataBase executeUpdate:sql,num,name,sex];
            if ( !result ) {
                NSLog(@"插入失败！");
                return;
            }
        }
    }
    @catch (NSException *exception) {
        isRollBack = YES;
        // 事务回退
        [dataBase rollback];
    }
    @finally {
        if (!isRollBack) {
            //事务提交
            [dataBase commit];
        }
    }

    [dataBase close];
    NSDate *date2 = [NSDate date];
    NSTimeInterval a = [date2 timeIntervalSince1970] - [date1 timeIntervalSince1970];
    NSLog(@"FMDatabase使用事务插入500条数据用时%.3f秒",a);
}

二: FMDatabase不使用事务的方法：

//二: FMDatabase不使用事务的方法：

-(void)noTransaction {
    NSDate *date1 = [NSDate date];

    // 创建表
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *dbPath = [docPath stringByAppendingPathComponent:@"mytable3.db"];
    NSLog(@"dbPath = %@",dbPath);
    FMDatabase *dataBase = [FMDatabase databaseWithPath:dbPath];

    // 注意这里的判断一步都不能少,特别是这里open的判断

    if (![dataBase open]) {
        NSLog(@"打开数据库失败");
        return ;
    }

    NSString *sqlStr = @"create table if not exists mytable3(num integer,name varchar(7),sex char(1),primary key(num));";
    BOOL res = [dataBase executeUpdate:sqlStr];
    if (!res) {
        NSLog(@"error when creating mytable1");

        [dataBase close];
    }

        for (int i = 0; i<500; i++) {
            NSNumber *num = @(i+1);
            NSString *name = [[NSString alloc] initWithFormat:@"student_%d",i];
            NSString *sex = (i%2==0)?@"f":@"m";

            NSString *sql = @"insert into mytable3(num,name,sex) values(?,?,?);";
            BOOL result = [dataBase executeUpdate:sql,num,name,sex];
            if ( !result ) {
                NSLog(@"插入失败！");
                return;
            }
        }


    [dataBase close];
    NSDate *date2 = [NSDate date];
    NSTimeInterval a = [date2 timeIntervalSince1970] - [date1 timeIntervalSince1970];
    NSLog(@"FMDatabase不使用事务插入500条数据用时%.3f秒",a);
}

是否使用事务的比较结果如下:

2017-01-18 00:28:57.455 Location[5319:300127] FMDatabase使用事务插入500条数据用时0.018秒
2017-01-18 00:28:58.509 Location[5319:300127] FMDatabase不使用事务插入500条数据用时1.054秒

三: FMDatabaseQueue使用事务的方法：

//多线程事务
- (void)transactionByQueue {
    NSDate *date1 = [NSDate date];

    // 创建表
    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *dbPath = [docPath stringByAppendingPathComponent:@"mytable2.db"];


    //多线程安全FMDatabaseQueue可以替代dataBase
    FMDatabaseQueue *queue = [FMDatabaseQueue databaseQueueWithPath:dbPath];

    //开启事务
    [queue inTransaction:^(FMDatabase *db, BOOL *rollback) {

        if(![db open]){

            return NSLog(@"事务打开失败");
        }

        NSString *sqlStr = @"create table mytable2(num integer,name varchar(7),sex char(1),primary key(num));";
        BOOL res = [db executeUpdate:sqlStr];
        if (!res) {
            NSLog(@"error when creating mytable2 table");

            [db close];
        }

        for (int i = 0; i<500; i++) {
            NSNumber *num = @(i+1);
            NSString *name = [[NSString alloc] initWithFormat:@"student_%d",i];
            NSString *sex = (i%2==0)?@"f":@"m";
            NSString *sql = @"insert into mytable2(num,name,sex) values(?,?,?);";
            BOOL result = [db executeUpdate:sql,num,name,sex];
            if ( !result ) {
                //当最后*rollback的值为YES的时候，事务回退，如果最后*rollback为NO，事务提交
                *rollback = YES;
                return;
            }
        }

      [db close];
    }];
    NSDate *date2 = [NSDate date];
    NSTimeInterval a = [date2 timeIntervalSince1970] - [date1 timeIntervalSince1970];
    NSLog(@"FMDatabaseQueue使用事务插入500条数据用时%.3f秒",a);
}


