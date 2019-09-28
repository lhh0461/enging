#当entity创建
def __ctor__(entity):
    #start timer
    add_timer(entity, "timer_cb", 10)

def rpc_host_alliance_req(entity, alliance_id, mailbox):
    temp = entity["temp_data"]
    
    print("rpc call success!!", __file__, alliance_id) 
    rpc.rpc_call(mailbox, "rpc_host_alliance_req", alliance_id)

def timer_cb(entity, alliance_id, mailbox):
