# 需解决问题
1、如何表示一个玩家?
2、如何表示一个玩法?
玩家和全局的玩法都是需要一个唯一标识的地址（serverid, entitytype, entityid）
3、像帮派应该是一个全局服务，我要取我的帮派信息，应该RPC过去帮派服务，调对应的业务函数

一个PyEntity代表一个实体对象，从此玩法、玩家都由PyEntity来表示
PyEntityObj为一个新定义的py对象，包含由entity类型,entityID,内存数据，存盘数据
玩家、帮派都是一个实体，可以选择加上存盘属性，存储的数据这就是组件

一个服务器集群里，有多个游戏服（GAMED），可能有多个战斗服（FIGHTD），可能还有多个场景服（SCENED）
游戏服之间相互连接，如何发现对方？
服务器起来后主动去连接centerd，centerd分配id和端口，返回给服务器，服务器做一些初始化，初始化成功后
返回给centerd一个初始化成功的消息，这时候加入集群，广播增加一个服的协议，给新加进来的服务器发送
当前的服务器列表信息，还有全局服务的信息。

#服务器框架
centerd
gated x N
gamed x N
fightd x N
dbd x N

#创建全局entity流程
调用create_entity，创建entity，创建成功后，调用register_global_entity，给centerd注册，
centerd回调到这边的register_global_entity_back接口，告知注册的结果，如果不成功，要清除
之前创建的entity

#时序问题
需要所有的全局entity服务初始化完毕，才对外服务

Entity有一个存盘对象，如果有该组件，则自动序列化存盘
所有的有存盘的Entity都要加上master和slave备份，他们之间如果master有修改马上同步到slave
