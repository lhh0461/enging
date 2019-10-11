class Player(Entity):
    _save_list = [
        "prop",
        "item",
        "equip",
    ]

    def __init__(self):
        pass

    def __on_create__(self):
        pass
    
    def timer_cb(self, alliance_id, mailbox):
        pass

p = Player()

