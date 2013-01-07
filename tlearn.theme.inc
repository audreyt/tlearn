<?php 

/**
 * @file
 * theming functions for the tlearn module.
 */

/**
 * Form submit callback for the tlearn troles.
 */
//function tlearn_admin_troles_submit($form, &$form_state) {
//  foreach ($form_state['values'] as $rid => $category) {
//    if (is_numeric($rid) && $form[$rid]['#trole']->weight != $trole['weight']) {
//      $form[$rid]['#trole']->weight = $trole['weight'];
//      tlearn_trole_save($form[$rid]['#trole']);
//    }
//  }
//  drupal_set_message(t('Your configuration has been saved.'));
//}



function theme_tlearn_admin_troles($variable) {

  $form = $variable['form'];
  
  $rows = array();
  
//  $sss = element_children($form);

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['name'])) {
      $trole = &$form[$key];

      $row = array();
      $row[] = drupal_render($trole['name']);
      
      if (isset($trole['weight'])) {
        $trole['weight']['#attributes']['class'] = array('tlearn-role-weight');
        $row[] = drupal_render($trole['weight']);
      }
      
      $row[] = drupal_render($trole['terms']); 
      $row[] = drupal_render($trole['edit']);
      $row[] = drupal_render($trole['delete']);
      
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }

  $header = array(t('Role'));

  if (isset($form['submit'])) {
    $header[] = t('Weight');
    drupal_add_tabledrag('tlearn-trole', 'order', 'self', 'tlearn-role-weight');
  }

  $header[] = '閱讀權限'; 
  $header[] = array('data' => '操作', 'colspan' => '2');
  
  return 
    theme('table', array(
      'header' => $header, 
      'rows' => $rows, 
      'empty' => t('尚未建立角色的閱讀權限。 <a href="@link">新增角色閱讀權限</a>.', array('@link' => url('admin/config/tlearn/troles/add'))),
      'attributes' => array('id' => 'tlearn-trole'),
      )
    )
    . drupal_render_children($form);
}





