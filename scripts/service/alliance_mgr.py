import XEngine
#帮派管理器

#当entity创建时调用
def __on_create__(entity):
    #start timer
    XEngine.add_timer(entity, "timer_cb", 10)

#当entity注册成功后调用
def __on_registered__(entity, result):
    pass

#当entity注册成功后调用
def __on_inited__(entity):
    #初始化成功
    pass

def rpc_host_alliance_req(entity, alliance_id, mailbox):
    temp = entity["temp_data"]
    
    print("rpc call success!!", __file__, alliance_id) 
    rpc.rpc_call(mailbox, "rpc_host_alliance_req", alliance_id)

def timer_cb(entity, alliance_id, mailbox):
