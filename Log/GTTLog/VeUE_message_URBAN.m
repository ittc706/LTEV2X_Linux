%�����·����

% % ��ʾĳ��RSU�²�ͬ���µĳ�������
[data1,data2,data3,data4,data5] = textread('VeUEMessage.txt','%n%n%n%n%n');
dim = length(data1);
ClusterNumber = 5;
RandomColor = rand(ClusterNumber,3);
 for cnt = 1:1:dim
    if(data2(cnt,1)==8)%ѡ��鿴��RSU��ţ���0��23
    ClusterId = data3(cnt,1)+1;
    plot(data4(cnt,1),data5(cnt,1),'X','Color',RandomColor(ClusterId,:));hold on
    end
 end
 
% ��ʾ��ͬRSU�³���������
% [data1,data2,data3,data4,data5] = textread('VeUEMessage.txt','%n%n%n%n%n');
% dim = length(data1);
% RSUNumber = 24;
% RandomColor = rand(RSUNumber,3);
%  for cnt = 1:1:dim
%     RSUId = data2(cnt,1)+1;
%     plot(data4(cnt,1),data5(cnt,1),'X','Color',RandomColor(RSUId,:));hold on
%  end
% 
%     
    
            