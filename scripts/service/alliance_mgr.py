import XEngine

#帮派管理器
class AllianceMgr(Entity):

    #当entity创建时调用
    def __on_create__(self):
        #start timer
        XEngine.add_timer(entity, "timer_cb", 10)
    
    #当entity注册成功后调用
    def __on_registered__(self, result):
        pass
    
    #当entity注册成功后调用
    def __on_inited__(self):
        #初始化成功
        pass
    
    def rpc_host_alliance_req(self, alliance_id, mailbox):
        temp = entity["temp_data"]
        
        print("rpc call success!!", __file__, alliance_id) 
        rpc.rpc_call(mailbox, "rpc_host_alliance_req", alliance_id)
    
    def timer_cb(self, alliance_id, mailbox):
        pass
