clear all;
close all;
clc;


separate=0;%������ͼ�ϲ����Ƿֿ�
figId=1;%ͼ��Id

%% ��ȡʱ��ͳ����Ϣ

EmergencyDelayStatistics=load('EmergencyDelayStatistics.txt');
PeriodDelayStatistics=load('PeriodDelayStatistics.txt');
DataDelayStatistics=load('DataDelayStatistics.txt');

EmergencyQueuingDelay=EmergencyDelayStatistics(1,:);
EmergencySendDelay=EmergencyDelayStatistics(2,:);

PeriodQueuingDelay=PeriodDelayStatistics(1,:);
PeriodSendDelay=PeriodDelayStatistics(2,:);

DataQueuingDelay=DataDelayStatistics(1,:);
DataSendDelay=DataDelayStatistics(2,:);

[numberEmergency,centerEmergency]=hist(EmergencyQueuingDelay,min(EmergencyQueuingDelay)-1:max(EmergencyQueuingDelay)+1);
numberEmergency=numberEmergency./sum(numberEmergency);

[numberPeriod,centerPeriod]=hist(PeriodQueuingDelay,min(PeriodQueuingDelay)-1:max(PeriodQueuingDelay)+1);
numberPeriod=numberPeriod./sum(numberPeriod);

[numberData,centerData]=hist(DataQueuingDelay,min(DataQueuingDelay)-1:max(DataQueuingDelay)+1);
numberData=numberData./sum(numberData);

%% �ȴ�ʱ��
if(separate==1)
    figure(figId)
    figId=figId+1;
    bar(centerEmergency,numberEmergency);
    title('�����¼��ȴ�ʱ��ͳ��','LineWidth',2);
    xlabel('�ȴ�ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;

    figure(figId)
    figId=figId+1;
    bar(centerPeriod,numberPeriod);
    title('�����¼��ȴ�ʱ��ͳ��','LineWidth',2);
    xlabel('�ȴ�ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
    
    figure(figId)
    figId=figId+1;
    bar(centerData,numberData);
    title('����ҵ���¼��ȴ�ʱ��ͳ��','LineWidth',2);
    xlabel('�ȴ�ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
else
    figure(figId)
    figId=figId+1;
    set(1,'position',[0,0,1800,600]);
    
    subplot(1,3,1);
    bar(centerEmergency,numberEmergency);
    title('�����¼��ȴ�ʱ��ͳ��','LineWidth',2);
    xlabel('�ȴ�ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
    
    subplot(1,3,2);
    bar(centerPeriod,numberPeriod);
    title('�����¼��ȴ�ʱ��ͳ��','LineWidth',2);
    xlabel('�ȴ�ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;

    subplot(1,3,3);
    bar(centerData,numberData);
    title('����ҵ���¼��ȴ�ʱ��ͳ��','LineWidth',2);
    xlabel('�ȴ�ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
end


%% ����ʱ��
[numberEmergency,centerEmergency]=hist(EmergencySendDelay,min(EmergencySendDelay)-1:max(EmergencySendDelay)+1);
numberEmergency=numberEmergency./sum(numberEmergency);

[numberPeriod,centerPeriod]=hist(PeriodSendDelay,min(PeriodSendDelay)-1:max(PeriodSendDelay)+1);
numberPeriod=numberPeriod./sum(numberPeriod);

[numberData,centerData]=hist(DataSendDelay,min(DataSendDelay)-1:max(DataSendDelay)+1);
numberData=numberData./sum(numberData);


if(separate==1)
    figure(figId)
    figId=figId+1;
    bar(centerEmergency,numberEmergency);
    title('�����¼�����ʱ��ͳ��','LineWidth',2);
    xlabel('����ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
    
    figure(figId)
    figId=figId+1;
    bar(centerPeriod,numberPeriod);
    title('�����¼�����ʱ��ͳ��','LineWidth',2);
    xlabel('����ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;

    figure(figId)
    figId=figId+1;
    bar(centerData,numberData);
    title('����ҵ���¼�����ʱ��ͳ��','LineWidth',2);
    xlabel('����ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
else
    figure(figId)
    figId=figId+1;
    set(2,'position',[0,0,1800,600]);
    
    subplot(1,3,1);
    bar(centerEmergency,numberEmergency);
    title('�����¼�����ʱ��ͳ��','LineWidth',2);
    xlabel('����ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
    
    subplot(1,3,2);
    bar(centerPeriod,numberPeriod);
    title('�����¼�����ʱ��ͳ��','LineWidth',2);
    xlabel('����ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;

    subplot(1,3,3);
    bar(centerData,numberData);
    title('����ҵ���¼�����ʱ��ͳ��','LineWidth',2);
    xlabel('����ʱ��(TTI)','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
end



%% ��ȡ��ͻͳ����Ϣ
EmergencyConflictNum=load('EmergencyConflictNum.txt');
PeriodConflictNum=load('PeriodConflictNum.txt');
DataConflictNum=load('DataConflictNum.txt');



[numberEmergency,centerEmergency]=hist(EmergencyConflictNum,min(EmergencyConflictNum)-1:max(EmergencyConflictNum)+1);
numberEmergency=numberEmergency./sum(numberEmergency);

[numberPeriod,centerPeriod]=hist(PeriodConflictNum,min(PeriodConflictNum)-1:max(PeriodConflictNum)+1);
numberPeriod=numberPeriod./sum(numberPeriod);

[numberData,centerData]=hist(DataConflictNum,min(DataConflictNum)-1:max(DataConflictNum)+1);
numberData=numberData./sum(numberData);

if(separate==1)
    figure(figId)
    figId=figId+1;
    bar(centerEmergency,numberEmergency);
    title('�����¼���ͻͳ��','LineWidth',2);
    xlabel('��ͻ����','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
    
    figure(figId)
    figId=figId+1;
    bar(centerPeriod,numberPeriod);
    title('�����¼���ͻͳ��','LineWidth',2);
    xlabel('��ͻ����','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;

    figure(figId)
    figId=figId+1;
    bar(centerData,numberData);
    title('����ҵ���¼���ͻͳ��','LineWidth',2);
    xlabel('��ͻ����','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
else
    figure(figId)
    figId=figId+1;
    set(3,'position',[0,0,1800,600]);

    subplot(1,3,1);
    bar(centerEmergency,numberEmergency);
    title('�����¼���ͻͳ��','LineWidth',2);
    xlabel('��ͻ����','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
    
    subplot(1,3,2);
    bar(centerPeriod,numberPeriod);
    title('�����¼���ͻͳ��','LineWidth',2);
    xlabel('��ͻ����','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;


    subplot(1,3,3);
    bar(centerData,numberData);
    title('����ҵ���¼���ͻͳ��','LineWidth',2);
    xlabel('��ͻ����','LineWidth',2);
    ylabel('����','LineWidth',2);
    grid on;
end




%% ������
TTIThroughput=load('./TTIThroughput.txt');
RSUThroughput=load('./RSUThroughput.txt');
VeUENumPerRSULogInfo=load('../GTTLog/VeUENumPerRSULogInfo.txt');

TTIThroughput=TTIThroughput/1000;
accumulatedTTIThroughput=zeros(1,length(TTIThroughput));
accumulatedTTIThroughput(1,1)=TTIThroughput(1,1);
for iter=2:length(TTIThroughput)
    accumulatedTTIThroughput(1,iter)=accumulatedTTIThroughput(1,iter-1)+TTIThroughput(1,iter);
end

figure(figId)
figId=figId+1;
plot(TTIThroughput);
title('TTI������','LineWidth',2);
xlabel('TTI','LineWidth',2);
ylabel('K bit','LineWidth',2);
grid on;

figure(figId)
figId=figId+1;
plot(accumulatedTTIThroughput);
title('�ۼ�������','LineWidth',2);
xlabel('TTI','LineWidth',2);
ylabel('K bit','LineWidth',2);
grid on;


RSUThroughput=RSUThroughput/1000;
figure(figId)
figId=figId+1;
bar(RSUThroughput);
title('������ͳ��','LineWidth',2);
xlabel('RSUId','LineWidth',2);
ylabel('K bit','LineWidth',2);
grid on;


VeUENumPerRSULogInfo=mean(VeUENumPerRSULogInfo,1);
figure(figId)
figId=figId+1;
bar(VeUENumPerRSULogInfo)
title('�����ֲ�ͳ��ͼ','LineWidth',2);
xlabel('������Ŀ','LineWidth',2);
ylabel('RSUId','LineWidth',2);
grid on;


%TTI������CDFͼ
[numberTTIThroughput,centerTTIThroughput]=hist(TTIThroughput,100);
Tem=zeros(length(centerTTIThroughput),2);
Tem(:,1)=centerTTIThroughput.';
Tem(:,2)=numberTTIThroughput.';
sortrows(Tem,1);%��������ֵ������ֵ����
centerTTIThroughput=Tem(:,1).';
numberTTIThroughput=Tem(:,2).';
TotalTTI=length(TTIThroughput);
accumulateNumberTTIThroughput=cumsum(numberTTIThroughput);%����������
accumulatePTTIThroughput=accumulateNumberTTIThroughput./TotalTTI;%�����۸���

figure(figId)
figId=figId+1;
plot(centerTTIThroughput,accumulatePTTIThroughput,'bo-','LineWidth',2);
title('TTI������CDFͼ','LineWidth',2);
xlabel('������(Kbits/TTI)','LineWidth',2);
ylabel('CDF','LineWidth',2);
grid on;


%RSU������CDFͼ
RSUThroughput=RSUThroughput/length(TTIThroughput);
[numberRSUThroughput,centerRSUThroughput]=hist(RSUThroughput,length(RSUThroughput));
Tem=zeros(length(centerRSUThroughput),2);
Tem(:,1)=centerRSUThroughput.';
Tem(:,2)=numberRSUThroughput.';
sortrows(Tem,1);%��������ֵ������ֵ����
centerRSUThroughput=Tem(:,1).';
numberRSUThroughput=Tem(:,2).';
TotalRSU=length(RSUThroughput);
accumulateNumberRSUThroughput=cumsum(numberRSUThroughput);%����������
accumulatePRSUThroughput=accumulateNumberRSUThroughput./TotalRSU;%�����۸���

figure(figId)
figId=figId+1;
plot(centerRSUThroughput,accumulatePRSUThroughput,'bo-','LineWidth',2);
title('RSU������CDFͼ','LineWidth',2);
xlabel('������(Kbits/TTI/RSU)','LineWidth',2);
ylabel('CDF','LineWidth',2);
grid on;



%% �����¼��Լ�����ҵ���¼����ɷֲ���֤
EmergencyPossion=load('./EmergencyPossion.txt');
DataPossion=load('./DataPossion.txt');

if(unique(EmergencyPossion)==0)
    [numberEmergency,centerEmergency]=hist(EmergencyPossion,10);
else
    [numberEmergency,centerEmergency]=hist(EmergencyPossion,unique(EmergencyPossion));
end


figure(figId)
figId=figId+1;
bar(centerEmergency,numberEmergency);
title('�����¼����ɷֲ�ͼ','LineWidth',2);
xlabel('�ڷ���ʱ���ڽ����¼����ɴ���','LineWidth',2);
ylabel('������Ŀ','LineWidth',2);
grid on;

if(unique(DataPossion)==0)
    [numberData,centerData]=hist(DataPossion,10);
else
    [numberData,centerData]=hist(DataPossion,unique(DataPossion));
end

figure(figId)
figId=figId+1;
bar(centerData,numberData);
title('����ҵ���¼����ɷֲ�ͼ','LineWidth',2);
xlabel('�ڷ���ʱ��������ҵ���¼����ɴ���','LineWidth',2);
ylabel('������Ŀ','LineWidth',2);
grid on;



