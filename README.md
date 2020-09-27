# ar_marker-docking
A to B navigation and docking project

printpose node를 실행하게 되면, 카메라에 ar_marker가 detection 되는지 확인 후 준비를 마칩니다.

Rviz에서 goal을 publish하면 move_base_simple/goal topic으로 받게됩니다. 

goal_status로 turtlebot이 경로를 향해 주행중인지, 도착했는지 판단 후에, 도착했다면 원래 있던 위치로 goal을 publish하여 돌아갑니다.

도착한 후, ar_marker의 좌표값을 받아 일정 거리 안으로 docking을 하여 끝이납니다.

카메라가 느린 문제를 해결하지 못하여 정확한 docking 알고리즘을 적용하지 못하였습니다.
