# Test Cases

## User login

### 用户名正确 密码错误

1. 用户名: John	密码:Pa@$w0rd
2. 用户名: admin       密码:lhzdnb

### 用户名错误 密码正确

1. 用户名: Johe	密码:Pa@$w0rd
2. 用户名: Victorie    密码:V1cT0rY9

### 用户名错误 密码错误

1. 用户名: Johe	密码:Pa@$word
2. 用户名: Zeka	密码:NoPassword

### 用户名正确 密码正确

1. 用户名: Gloria      密码:sunSh1ne
2. 用户名: admin	密码:Admin

## Server

### 书名正确, 数量大于0

1. S server
   1. S305
   2. S210
2. L server
   1. L301
   2. L102
3. H server
   1. H104
   2. H201

### 书名正确, 数量=0

1. S server
   1. S209
2. L server
   1. L210
3. H server
   1. H305

### 书名错误

1. S server
   1. S309
2. L server
   1. L208
3. H server
   1. H203