from ctrlset_ipbusbar.ipbusbar_web import  *

class IpBusbar(IpBusbarWeb):

    def start_fun(self):
        #self.ip_prefix = 'https://'
        self.login()
        self.setCorrect()
        #self.login()
        #self.setEle()
        #self.setThreshold()
        #self.clearLogs()
        #self.resetFactory()
        #self.reboot()
        self.driver.quit()

    def setTime(self):
        self.divClick(8)
        self.execJsAlert("check_time()")
        self.sendtoMainapp("设置设备时间", 1)

    def clearLogs(self):
        en = self.cfgs['log_en']
        if(int(en) < 1):
            return
        self.setTime()
        self.divClick(9)
        jsSheet = "xmlset = createXmlRequest();xmlset.onreadystatechange = setdata;ajaxgets(xmlset, \"/setlclear?a=\" + {0} + \"&\");"
        for num in range(0, 2):
            self.setSelect("loglist", num)
            self.execJs(jsSheet.format(num))
            time.sleep(1)
        self.sendtoMainapp("设备日志清除成功", 1)

    def setCorrect(self):
        cfg = self.cfgs
        ip = self.ip_prefix + cfg['ip'] + '/correct.html'
        self.driver.get(ip); time.sleep(1)
        self.driver.switch_to.default_content()

        if (len(cfg['mac']) > 5):
            self.setItById("mac1", cfg['mac'], 'Mac地址')
        self.alertClick("Button3")
        self.sendtoMainapp("设备后台网页配置成功", 1)
        self.driver.back()
        time.sleep(0.5)

    





